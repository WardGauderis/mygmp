#ifndef IGMPCLIENTFILTER_HH
#define IGMPCLIENTFILTER_HH

#include <click/element.hh>
CLICK_DECLS

// TODO RFC-5: receive 224.0.0.1 voor query
// TODO RFC-5: always enable 244.0.0.1; no IGMP messages about this address
// filter router-alert?

class IGMPClientFilter {};

CLICK_ENDDECLS
#endif    // IGMPCLIENTFILTER_HH
