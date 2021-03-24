#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "IGMPClientState.hh"
#include "IGMPMessages.hh"

CLICK_DECLS

bool IGMPClientState::addAddress(IPAddress address) {
	if (hasAddress(address)) return false;
	addresses.push_back(address);
	return true;
}

bool IGMPClientState::removeAddress(IPAddress address) {
	for (auto& a : addresses) {
		if (a == address) {
			std::swap(a, addresses.back());
			addresses.pop_back();
			return true;
		}
	}
	return false;
}

bool IGMPClientState::hasAddress(IPAddress address) {
	for (const auto& a : addresses) {
		if (a == address) return true;
	}
	return false;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPClientState)
