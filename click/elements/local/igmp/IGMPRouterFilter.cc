#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPRouterFilter.hh"

CLICK_DECLS
int IGMPRouterFilter::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
            .read_mp("STATE", ElementCastArg("IGMPRouterState"), state)
            .complete()) {
        return errh->error("Could not parse IGMPRouterState");
    }

    // TODO: general queries??

    return 0;
}

void IGMPRouterFilter::push(int input, Packet *packet) {
    // Idk if this actually doesn't happen, just for safety
    if (input < 0) return;
	click_chatter("router filter received packet on interface %u", input);

    // group address
    auto address = IPAddress(packet->ip_header()->ip_dst);

    for (auto &interface: state->interfaces) {
        // This means this specific interface doesn't recognise the group address.
        if (interface.second.find(address) == interface.second.end()) continue;

        auto& group = interface.second[address];
        click_chatter("in filter: %s -> %s", address.unparse().c_str(), group.isExclude ? "true" : "false");

        // Check if someone wants this by looking if mode for group is exclude
        if (group.isExclude) {
            output(int(interface.first)).push(packet->clone());
        }
    }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouterFilter)
