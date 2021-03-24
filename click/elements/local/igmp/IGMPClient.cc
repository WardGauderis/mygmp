#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
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

void IGMPClient::push(int, Packet* p) {}

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
		click_chatter("Joined %s", address.unparse().c_str());
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
		click_chatter("Left %s", address.unparse().c_str());
	}

	if (client->state->removeAddress(address))
		client->scheduleStateChangeMessage(CHANGE_TO_INCLUDE_MODE, address);

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

	if (qrv <= 1) return;

	auto* timerdata = new ScheduledReport{ this, packet, qrv - 1 };
	auto* timer     = new Timer(&handleReport, timerdata);
	timer->initialize(this);
	timer->schedule_after_msec((float) rand() / (float) RAND_MAX * unsolicitedReportInterval);

	return;
}

void IGMPClient::handleReport(Timer* timer, void* data) {
	auto* report = (ScheduledReport*) data;
	assert(report);

	report->client->output(0).push(report->packet->clone());
	if (report->remaining <= 1) return;
	report->remaining--;
	timer->schedule_after_msec((float) rand() / (float) RAND_MAX *
	                           report->client->unsolicitedReportInterval);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClient)
