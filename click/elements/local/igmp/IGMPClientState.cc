#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPClientState.hh"
#include "IGMPMessages.hh"
#include <algorithm>

CLICK_DECLS

bool IGMPClientState::addAddress(IPAddress address) {
	if (address == IPAddress("224.0.0.1")) return false;
	return addresses.insert(address).second; }

bool IGMPClientState::removeAddress(IPAddress address) {
	return addresses.erase(address); }

bool IGMPClientState::hasAddress(IPAddress address) const {
	return addresses.count(address);
}

bool IGMPClientState::hasState() const {
	return !addresses.empty();
}
size_t IGMPClientState::size() const { return addresses.size(); }

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClientState)
