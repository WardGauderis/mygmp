#ifndef IGMPCLIENTSTATE_HH
#define IGMPCLIENTSTATE_HH

#include <click/element.hh>
#include <unordered_set>
CLICK_DECLS

class IGMPClientState : Element {
public:
	const char* class_name() const override { return "IGMPClientState"; }
	const char* port_count() const override { return "0"; }

	bool addAddress(IPAddress address);

	bool removeAddress(IPAddress address);

private:
	// RFC-3.2: interface state
	// state is only a set because the client has only one interface and socket and the source-list is always empty
	// addresses in the set have EXCLUDE {}
	// addresses outside the set have INCLUDE {}
	std::unordered_set<IPAddress> addresses;
};

CLICK_ENDDECLS

#endif    // IGMPCLIENTSTATE_HH
