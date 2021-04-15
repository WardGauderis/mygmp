#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPClientState.hh"
#include "IGMPMessages.hh"

CLICK_DECLS

/**
 * join an address
 * @param address
 * @return True if newly joined
 */
bool IGMPClientState::addAddress(IPAddress address) {
	if (address == IPAddress("224.0.0.1")) return false;
	return addresses.insert(address).second;
}

/**
 * Leave an address
 * @param address
 * @return True if newly left
 */
bool IGMPClientState::removeAddress(IPAddress address) { return addresses.erase(address); }

/**
 * check if address has been joined
 * @param address
 * @return
 */
bool IGMPClientState::hasAddress(IPAddress address) const {
	if (address == IPAddress("224.0.0.1")) return true;
	return addresses.count(address);
}

/**
 * check if any address has been joined
 * @return
 */
bool IGMPClientState::hasState() const { return !addresses.empty(); }

/**
 * get the amount of joined addresses
 * @return
 */
size_t IGMPClientState::size() const { return addresses.size(); }

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClientState)
