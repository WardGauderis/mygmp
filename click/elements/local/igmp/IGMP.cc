#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMP.hh"

CLICK_DECLS
int IGMP::configure(Vector<String>& conf, ErrorHandler* errh) { return 0; }

void IGMP::push(int, Packet* p) { output(0).push(p); }

Packet* IGMP::pull(int) { return input(0).pull(); }

void IGMP::add_handlers() {
	add_write_handler("join", &handleJoin, nullptr);
	add_write_handler("leave", &handleSetInterval, nullptr);
}

int IGMP::handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh) {
	auto state = (IGMP*) e;
	return 0;
}

int IGMP::handleLeave(const String& conf, Element* e, void* thunk, ErrorHandler* errh) {
	auto state = (IGMP*) e;
	return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMP)
