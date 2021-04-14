#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
#include "IGMPRouter.hh"

CLICK_DECLS
int IGMPRouter::configure(Vector<String>& conf, ErrorHandler* errh) {
	if (Args(conf, this, errh)
	        .read_mp("STATE", ElementCastArg("IGMPRouterState"), state)
	        .complete()) {
		return errh->error("Could not parse IGMPRouterState");
	}

	auto data  = new std::pair<IGMPRouter*, uint32_t>(this, state->startupQueryCount);
	auto timer = new Timer(IGMPRouter::handleGeneralResend, data);

	// Weird trick with the direct schedule to reduce code duplication
	timer->initialize(this);
	timer->schedule_after_msec(0);

	return 0;
}

void IGMPRouter::push(int input, Packet* packet) {
	auto report = (ReportMessage*) (packet->data() + packet->ip_header_length());

	// Idk if this actually doesn't happen, just for safety
	if (input < 0) return;

	// check for alert option
	RouterAlertOption option{};
	if (!(packet->ip_header_length() > 5 * 4 &&
	      !memcmp((packet->data() + packet->ip_header_length() - 4), &option,
	              sizeof(RouterAlertOption)))) {
		packet->kill();
		click_chatter("Dropped packet without alert option");
		return;
	}
	// check for bad checksum
	auto length = sizeof(ReportMessage) + ntohs(report->NumGroupRecords) * sizeof(GroupRecord);
	if (click_in_cksum((const unsigned char*) report, int(length))) {
		packet->kill();
		click_chatter("Dropped wrong checksum packet in router.");
		return;
	}
	// check for report
	if (report->type != REPORT) {
		packet->kill();
		return;
	}

	// process and kill packet
	processReport(report, static_cast<uint32_t>(input));
	packet->kill();
}

void IGMPRouter::processReport(ReportMessage* report, uint32_t interface) {
	// create the interface if it doesn't exist
	if (state->interfaces.find(interface) == state->interfaces.end())
		state->interfaces.emplace(interface, Groups{});

	for (auto i = 0; i < ntohs(report->NumGroupRecords); i++) {
		GroupRecord* record  = ((GroupRecord*) (report + 1)) + i;
		const auto   address = IPAddress(record->multicastAddress);

		// check if host asked for a valid multicast address
		if (!address.is_multicast()) continue;

		// create the group if it doesn't exist
		if (state->interfaces[interface].find(address) == state->interfaces[interface].end()) {
			auto data  = new std::pair<Groups*, IPAddress>(&state->interfaces[interface], address);
			auto timer = new Timer(IGMPRouter::groupExpire, data);

			// start the timer with this expiry time to delete the group
			timer->initialize(this);
			timer->schedule_after_msec(state->groupMembershipInterval * 100);

			state->interfaces[interface].emplace(address, GroupData{ timer, false });
		}

		auto& group = state->interfaces[interface][address];

		if (record->recordType == RecordType::MODE_IS_EXCLUDE or
		    record->recordType == RecordType::CHANGE_TO_EXCLUDE_MODE) {
			// Exclude {} -> Someone wants to listen so we set it to true
			group.isExclude = true;

			// Reset the group timer to the expiry as we know at least someone is listening
			group.groupTimer->schedule_after_msec(state->groupMembershipInterval * 100);;

		} else if (group.isExclude and not group.groupTimer->scheduled()) {
			// this is only triggered when the router doesn't know if someone is listening
			// and hasn't yet started the procedure to remedy this.

			// create the timer state
			auto data = new TimerData{ this, interface, address, state->lastMemberQueryCount };

			// this useful comment tells you the next line start a timer that sends a group specific
			// query
			auto timer = new Timer(IGMPRouter::handleSpecificResend, data);

			// timer of 0 seconds may sound weird but it avoids a lot of code duplication
			timer->initialize(this);
			timer->schedule_after_msec(0);

			// change group timer value
			group.groupTimer->schedule_after_msec(state->lastMemberQueryTime * 100);
		}
		else if(group.isExclude and group.groupTimer->scheduled())
		{
			// reset the timer in this case but do not send queries
            group.groupTimer->schedule_after_msec(state->lastMemberQueryTime * 100);
		}
		// If the mode is already include we don't have to worry about anything :)
	}
}

void IGMPRouter::groupExpire(Timer* timer, void* data) {
	auto* state = (std::pair<Groups*, IPAddress>*) data;

	// for safety
	click_chatter("removed group %s", state->second.unparse().c_str());
	(*state->first)[state->second].isExclude = false;

	// remove the group record
	state->first->erase(state->second);
}

void IGMPRouter::handleSpecificResend(Timer* timer, void* data) {
	auto values = (TimerData*) (data);

	values->numResends--;
	if (values->numResends == 0) return;

	sendGroupSpecificQuery(values->self, values->interface, values->address);
	timer->schedule_after_msec(values->self->state->lastMemberQueryInterval * 100);
}

void IGMPRouter::handleGeneralResend(Timer* timer, void* data) {
	auto* state = (std::pair<IGMPRouter*, uint32_t>*) (data);
	sendGeneralQueries(state->first);

	if (state->second > 0) {
		state->second--;
		timer->schedule_after_msec(state->first->state->startupQueryInterval * 100);
	} else {
		timer->schedule_after_msec(state->first->state->queryInterval * 100);
	}
}

void IGMPRouter::sendGroupSpecificQuery(IGMPRouter* self, uint32_t interface, IPAddress address) {
	auto msg = QueryMessage{
		MessageType::QUERY,      U32toU8(self->state->lastMemberQueryInterval), 0, address, 0, 0,
		self->state->robustness, U32toU8(self->state->queryInterval),           0
	};

	msg.checksum = click_in_cksum((const unsigned char*) (&msg), sizeof(QueryMessage));
	auto packet  = Packet::make(sizeof(click_ether) + sizeof(click_ip), &msg, sizeof(msg), 0);

	self->output(int(interface)).push(packet);
}

void IGMPRouter::sendGeneralQueries(IGMPRouter* self) {
	auto msg = QueryMessage{ MessageType::QUERY,
		                     U32toU8(self->state->queryResponseInterval),
		                     0,
		                     0,    // address is 0 in general query
		                     0,
		                     0,
		                     self->state->robustness,
		                     U32toU8(self->state->queryInterval),
		                     0 };

	msg.checksum = click_in_cksum((const unsigned char*) (&msg), sizeof(QueryMessage));
	auto packet  = Packet::make(sizeof(click_ether) + sizeof(click_ip), &msg, sizeof(msg), 0);

	// TODO: get the amount of interfaces and such in a generic way
	for (int i = 0; i < 3; i++) { self->output(i).push(packet->clone()); }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouter)
