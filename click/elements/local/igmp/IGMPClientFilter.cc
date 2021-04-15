#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPClientFilter.hh"
#include "IGMPMessages.hh"

CLICK_DECLS
/**
 * read the reference to the IGMPClientState
 * @param conf
 * @param errh
 * @return
 */
int IGMPClientFilter::configure(Vector<String>& conf, ErrorHandler* errh) {
	if (Args(conf, this, errh)
	        .read_mp("STATE", ElementCastArg("IGMPClientState"), state)
	        .complete()) {
		return errh->error("Could not parse IGMPClientState");
	}

	return 0;
}

/**
 * forward the packet to port 0 if it's required by the IGMPClientState
 * @param port
 * @param p
 */
void IGMPClientFilter::push(int port, Packet* p) {
	if (p->dst_ip_anno() == IPAddress("224.0.0.1") || state->hasAddress(p->dst_ip_anno())) {
		output(0).push(p);
	}
	output(1).push(p);
}
CLICK_ENDDECLS

EXPORT_ELEMENT(IGMPClientFilter)
