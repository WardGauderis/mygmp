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

void IGMPClient::push(int, Packet* p) { click_chatter("AANGEKOMEN"); }

int IGMPClient::handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh) {
	auto client = (IGMPClient*) e;

	Vector<String> vconf;
	cp_argvec(conf, vconf);

	IPAddress address;

	if (Args(vconf, client, errh).read_mp("ADDRESS", address).complete() < 0) {
		return errh->error("Could not parse multicast-address");
	}

	if (client->state->addAddress(address))
		client->scheduleStateChangeMessage(CHANGE_TO_EXCLUDE_MODE, address);

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

	if (client->state->removeAddress(address))
		client->scheduleStateChangeMessage(CHANGE_TO_INCLUDE_MODE, address);

	return 0;
}

ReportMessage* IGMPClient::scheduleStateChangeMessage(RecordType type, IPAddress address) {
	auto packet = Packet::make(sizeof(click_ether) + sizeof(click_ip), 0,
	                           sizeof(ReportMessage) + sizeof(GroupRecord), 0);

	if (!packet) {
		click_chatter("Could not allocate packet");
		return nullptr;
	}
	memset(packet->data(), 0, packet->length());

	auto header  = (ReportMessage*) packet->data();
	header->type = REPORT;
	// TODO try without htons and wireshark
	header->NumGroupRecords = htons(1);

	auto record              = (GroupRecord*) (header + 1);
	record->recordType       = type;
	record->multicastAddress = address.in_addr();

	output(0).push(packet);

	// if te klein
	//	auto* timerdata = new ScheduledReport{ this, header, qrv - 1 };
	//	auto* t         = new Timer(&handleReport, timerdata);
	//	t->initialize(this);
	//	t->schedule_after_msec((float) rand() / (float) RAND_MAX * unsolicitedReportInterval);

	return 0;
}

void IGMPClient::handleReport(Timer* timer, void* data) {
	auto* report = (ScheduledReport*) data;
	assert(report);
	//	report->client->handle(report)
	//	output(0)
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClient)
