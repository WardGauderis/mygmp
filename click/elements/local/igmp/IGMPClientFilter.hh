#ifndef IGMPCLIENTFILTER_HH
#define IGMPCLIENTFILTER_HH

#include <click/element.hh>
#include "IGMPClientState.hh"
CLICK_DECLS

class IGMPClientFilter: public Element {
public:
	const char* class_name() const override { return "IGMPClientFilter"; }
	const char* port_count() const override { return "1/2"; }
	const char* processing() const override { return PUSH; }

	int configure(Vector<String>&, ErrorHandler*) override;

	void push(int port, Packet* p) override;

private:
	IGMPClientState* state;
};

CLICK_ENDDECLS
#endif    // IGMPCLIENTFILTER_HH
