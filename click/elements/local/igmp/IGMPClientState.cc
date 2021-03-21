#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPClientState.hh"
#include "IGMPMessages.hh"

CLICK_DECLS

bool IGMPClientState::addAddress(IPAddress address) { return addresses.insert(address).second; }

bool IGMPClientState::removeAddress(IPAddress address) { return addresses.erase(address); }

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClientState)
