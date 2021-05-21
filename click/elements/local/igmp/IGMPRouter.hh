#ifndef IGMPROUTER_HH
#define IGMPROUTER_HH

#include <click/element.hh>
#include "IGMPRouterState.hh"
#include "IGMPMessages.hh"

// terminated group membership report -> query network before deleting group

class IGMPRouter;

struct TimerData {
	IGMPRouter* self;
	uint32_t    interface;
	IPAddress   address;
	uint32_t    numResends;
	bool first;
};

CLICK_DECLS
class IGMPRouter: public Element {
public:
	const char* class_name() const override { return "IGMPRouter"; }

	const char* port_count() const override { return "-/="; }

	const char* processing() const override { return PUSH; }

	int configure(Vector<String>&, ErrorHandler*) override;

	void push(int, Packet*) override;

	void processReport(ReportMessage* report, uint32_t interface);

	static void groupExpire(Timer*, void*);

	static void handleSpecificResend(Timer*, void*);

	static void handleGeneralResend(Timer*, void*);

	static void sendGroupSpecificQuery(IGMPRouter* self, uint32_t interface, IPAddress address);

	static void sendGeneralQueries(IGMPRouter* self);

private:
	IGMPRouterState* state;
};

CLICK_ENDDECLS

#endif    // IGMPROUTER_HH
