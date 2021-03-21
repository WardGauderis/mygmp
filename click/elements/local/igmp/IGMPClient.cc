#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <timer.hh>
#include "IGMPClient.hh"

CLICK_DECLS
int IGMPClient::configure(Vector<String>& conf, ErrorHandler* errh) { return 0; }

void IGMPClient::add_handlers() {
	add_write_handler("join", &handleJoin, nullptr);
	add_write_handler("leave", &handleLeave, nullptr);
}

void IGMPClient::push(int, Packet* p) { output(0).push(p); }

int IGMPClient::handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh) {
	auto client = (IGMPClient*) e;

	Vector<String> vconf;
	cp_argvec(conf, vconf);

	IPAddress address;

	if (Args(vconf, client, errh).read_mp("ADDRESS", address).complete() < 0) {
		return errh->error("Could not parse multicast-address");
	}

	if (client->state->removeAddress(address))
		client->scheduleStateChangeMessage(CHANGE_TO_EXCLUDE_MODE, address);

	return 0
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

	return 0
}

ReportMessage* IGMPClient::scheduleStateChangeMessage(RecordType type, IPAddress address) {
	//	TODO RFC-2: IPMulticastListen(address, INCLUDE/EXCLUDE, {})

	// TODO send one packet before scheduling

	auto* timerdata = new ScheduledReport{ this, nullptr, qrv };
	auto* t         = new Timer(&handleReport, timerdata);
	t->initialize(this);
	t->schedule_after_msec((float) rand() / (float) RAND_MAX * unsolicitedReportInterval);

	return 0;
}

void IGMPClient::handleReport(Timer* timer, void* data) {
	auto* report = (ScheduledReport*) data;
	assert(report);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClient)
