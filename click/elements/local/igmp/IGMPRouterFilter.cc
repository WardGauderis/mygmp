#include <click/config.h>
#include "IGMPRouterFilter.hh"

CLICK_DECLS
void IGMPRouterFilter::push(int input, Packet *packet) {
    // Idk if this actually doesn't happen, just for safety
    if (input < 0) return;

    // group address
    auto address = packet->ip_header()->ip_dst;

    for (auto &interface: state->interfaces) {
        // This means this specific interface doesn't recognise the group address.
        if (interface.second.find(address) == interface.second.end()) return;

        auto &group = interface.second[address];

        // Check if someone wants this by looking if mode for group is exclude
        if (std::get<2>(group)) {
            output(int(interface.first)).push(packet);
        }
    }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouterFilter)
