#ifndef CLICK_IGMPROUTERFILTER_HH
#define CLICK_IGMPROUTERFILTER_HH

#include <click/element.hh>
#include "IGMPRouterState.hh"

CLICK_DECLS

class IGMPRouterFilter: public Element {
public:
	const char* class_name() const override { return "IGMPRouterFilter"; }

	const char* port_count() const override { return "1/-"; }

	const char* processing() const override { return PUSH; }

	int configure(Vector<String>&, ErrorHandler*) override;

	void push(int, Packet*) override;

private:
	IGMPRouterState* state;
};

CLICK_ENDDECLS
#endif
