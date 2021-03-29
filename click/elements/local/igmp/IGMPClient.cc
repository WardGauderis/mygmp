#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
#include <string>
#include "IGMPClient.hh"

CLICK_DECLS
int IGMPClient::configure(Vector<String>& conf, ErrorHandler* errh) {
	if (Args(conf, this, errh)
	        .read_mp("STATE", ElementCastArg("IGMPClientState"), state)
	        .complete()) {
		return errh->error("Could not parse IGMPClientState");
	}

	return 0;
}

void IGMPClient::add_handlers() {
	add_write_handler("join", &handleJoin, nullptr);
	add_write_handler("leave", &handleLeave, nullptr);
}

// TODO RFC-5.2: reception of query

// on query: schedule response in ]0, MaxRespTime[ or merge if existing
// general interface timer, per-group timer
// 1. sooner response to general -> no
// 2. general -> schedule + cancel
// 3. group + no pending -> schedule
// 4. group + pending -> schedule min(pending, new)

// on general timer expiration:
// send report with all records

// on group timer expiration:
// send report with one record if exists

void IGMPClient::push(int, Packet* p) {
	// TODO good packet
	auto query = (QueryMessage*) (((click_ip*) p->data()) + 1);

	if (!state->hasState()) return;
	auto delay = (float) rand() / (float) RAND_MAX * query->maxRespTime();

	if (generalTimer->scheduled() &&
	    generalTimer->expiry_steady() - Timestamp::now_steady() < delay) {
		return;
	} else if (query->groupAddress == 0) {
		generalTimer->schedule_after_msec(delay);
		// TODO assign
	} else if (!groupTimers.count(query->groupAddress)) {
		groupTimers[query->groupAddress]->schedule_after_msec(delay);
		// TODO assign
	} else if (groupTimers[query->groupAddress]->expiry_steady() - Timestamp::now_steady() > delay){
		groupTimers[query->groupAddress]->schedule_after_msec(delay);
		// TODO assign
	}
}

int IGMPClient::handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh) {
	auto client = (IGMPClient*) e;

	Vector<String> vconf;
	cp_argvec(conf, vconf);

	IPAddress address;

	if (Args(vconf, client, errh).read_mp("ADDRESS", address).complete() < 0) {
		return errh->error("Could not parse multicast-address");
	}

	if (client->state->addAddress(address)) {
		client->scheduleStateChangeMessage(CHANGE_TO_EXCLUDE_MODE, address);
	}

	return 0;
}

int IGMPClient::handleLeave(const String& conf, Element* e, void* thunk, ErrorHandler* errh) {
	auto client = (IGMPClient*) e;

	Vector<String> vconf;
	cp_argvec(conf, vconf);

	IPAddress address;

	if (Args(vconf, client, errh).read_mp("ADDRESS", address).complete() < 0) {
		return errh->error("Could not parse multicast-address");
	}

	if (client->state->removeAddress(address)) {
		client->scheduleStateChangeMessage(CHANGE_TO_INCLUDE_MODE, address);
	}

	return 0;
}

void IGMPClient::scheduleStateChangeMessage(RecordType type, IPAddress address) {
	auto packet = Packet::make(sizeof(click_ether) + sizeof(click_ip), 0,
	                           sizeof(ReportMessage) + sizeof(GroupRecord), 0);

	if (!packet) {
		click_chatter("Could not allocate packet");
		return;
	}
	memset(packet->data(), 0, packet->length());

	auto header  = (ReportMessage*) packet->data();
	header->type = REPORT;
	// TODO try without htons and wireshark
	header->NumGroupRecords = htons(1);

	auto record              = (GroupRecord*) (header + 1);
	record->recordType       = type;
	record->multicastAddress = address.in_addr();

	header->checksum = click_in_cksum((const unsigned char*) (header),
	                                  sizeof(ReportMessage) + sizeof(GroupRecord));

	output(0).push(packet->clone());
	printMessage(std::to_string(qrv - 1) + " remaining", (ReportMessage*) header);

	auto iter = ChangeTimers.find(address);
	if (iter != ChangeTimers.end()) { (*iter).second->clear(); }

	if (qrv <= 1) return;

	auto* timerdata = new ScheduledReport{ this, packet, qrv - 1 };
	auto* timer     = new Timer(&handleReport, timerdata);
	timer->initialize(this);
	timer->schedule_after_msec((float) rand() / (float) RAND_MAX * unsolicitedReportInterval);
	ChangeTimers[address] = timer;
}

void IGMPClient::handleReport(Timer* timer, void* data) {
	auto* report = (ScheduledReport*) data;
	assert(report);
	report->client->output(0).push(report->packet->clone());
	printMessage(std::to_string(report->remaining - 1) + " remaining",
	             (ReportMessage*) report->packet->data());
	if (--report->remaining <= 0) {
		timer->clear();
		return;
	}
	timer->schedule_after_msec((float) rand() / (float) RAND_MAX *
	                           report->client->unsolicitedReportInterval);
}

void printMessage(std::string front, ReportMessage* message) {
	click_chatter("%s:\treport", front.c_str());
	for (uint16_t i = 0; i < ntohs(message->NumGroupRecords); ++i) {
		auto        record = (GroupRecord*) (message + 1) + i;
		std::string type;
		switch (record->recordType) {
		case MODE_IS_INCLUDE: type = "is_inc"; break;
		case MODE_IS_EXCLUDE: type = "is_exc"; break;
		case CHANGE_TO_INCLUDE_MODE: type = "to_inc"; break;
		case CHANGE_TO_EXCLUDE_MODE: type = "to_exc"; break;
		}
		click_chatter("\t%s %s", type.c_str(),
		              IPAddress(record->multicastAddress).unparse().c_str());
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClient)
