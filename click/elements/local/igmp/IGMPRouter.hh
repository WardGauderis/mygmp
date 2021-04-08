#ifndef IGMPCLIENT_HH
#define IGMPROUTER_HH

#include <click/element.hh>
#include "IGMPRouterState.hh"
#include "IGMPMessages.hh"

//TODO REF-4.2.13: accept 0.0.0.0
//TODO REF-6: accept 224.0.0.22

// terminated group membership report -> query network before deleting group

CLICK_DECLS
class IGMPRouter : public Element {
public:
    const char* class_name() const override { return "IGMPRouter"; }
    const char* port_count() const override { return "-/="; }
    const char* processing() const override { return PUSH; }

    int configure(Vector<String>&, ErrorHandler*) override;

    void push(int, Packet*) override;

    void processReport(ReportMessage* report, uint32_t interface);
    void sendQuery();

private:
//    IGMPRouterState* state;
};
CLICK_ENDDECLS


#endif //IGMPROUTER_HH
