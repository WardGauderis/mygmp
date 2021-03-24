#ifndef IGMPCLIENTFILTER_HH
#define IGMPCLIENTFILTER_HH

#include <click/element.hh>
CLICK_DECLS

// TODO RFC-5: receive 224.0.0.1 voor query
// TODO RFC-5: always enable 244.0.0.1; no IGMP messages about this address
// filter router-alert?

class IGMPClientFilter : public Element {
	const char* class_name() const override { return "IGMPClientFilter"; }
	const char* port_count() const override { return "1/1"; }
	const char* processing() const override { return PUSH; }
};

CLICK_ENDDECLS
#endif    // IGMPCLIENTFILTER_HH
