//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_IGMP_H
#define CLICK_IGMP_H

// TODO decide elements and names:
// client:	1. reception of query -> send report	-- IGMPClientElement
//			2. interface change -> send report		-- IGMP
//			3. filter packet						-- IGMPFilterElement
// router:	4. timer expired -> send query			-- IGMPRouterElement
//			5. reception of report -> state change	-- IGMPRouterElement
//			6. reception of data -> route			-- IGMPRoutingTable
//			3. filter packet						-- IGMPFilterElement

#include <click/element.hh>
CLICK_DECLS
class IGMP: public Element {
public:
	const char* class_name() const override { return "IGMP"; }
	const char* port_count() const override { return "1/1"; }
	const char* processing() const override { return PUSH; }

	int  configure(Vector<String>&, ErrorHandler*) override;
	void add_handlers() override;

	static int handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh);
	static int handleLeave(const String& conf, Element* e, void* thunk, ErrorHandler* errh);

	void push(int, Packet*) override;

private:
	// TODO RFC-3.2: interface state
	// TODO RFC-5.1: change of interface state

	// default include {}
	// TO_IN, TO_EX
	// retransmitted robust -1 time with interval ]0, unsolicited report interval[
	// new change before retransmissions done -> merge state-change report instead
	// merge report is new TO_IN of TO_EX in our case?
};

CLICK_ENDDECLS
#endif    // CLICK_IGMP_H