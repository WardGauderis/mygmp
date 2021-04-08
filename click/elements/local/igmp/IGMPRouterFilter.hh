#ifndef CLICK_IGMPROUTERFILTER_HH
#define CLICK_IGMPROUTERFILTER_HH

#include <click/element.hh>
#include "IGMPRouterState.hh"

CLICK_DECLS

class IGMPRouterFilter : public Element {
public:
    const char *class_name() const override { return "IGMPRouterFilter"; }

    const char *port_count() const override { return "-/="; }

    const char *processing() const override { return PUSH; }

    void push(int, Packet *) override;

private:
    IGMPRouterState *state;
};

CLICK_ENDDECLS
#endif
