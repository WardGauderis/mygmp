#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPClientFilter.hh"
#include "IGMPMessages.hh"

CLICK_DECLS

CLICK_ENDDECLS
int IGMPClientFilter::configure(Vector<String>& conf, ErrorHandler* errh) {
	if (Args(conf, this, errh)
	        .read_mp("STATE", ElementCastArg("IGMPClientState"), state)
	        .complete()) {
		return errh->error("Could not parse IGMPClientState");
	}

	return 0;
}

void IGMPClientFilter::push(int port, Packet* p){
	if (state->hasAddress(p->dst_ip_anno())) { output(0).push(p); }
	output(1).push(p);
}

EXPORT_ELEMENT(IGMPClientFilter)
