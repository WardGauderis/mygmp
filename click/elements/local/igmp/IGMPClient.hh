#ifndef IGMPCLIENT_H
#define IGMPCLIENT_H

#include <click/element.hh>
#include "IGMPMessages.hh"
#include "IGMPClientState.hh"
#include <string>
#include <unordered_map>
CLICK_DECLS

// client:	1. reception of query / interface change -> send report	-- IGMPClient
//			2. filter IP packets									-- IGMPClientFilter
//			3. state												-- IGMPClientState

// TODO decide elements and names:
// router:	4. timer expired -> send query			-- IGMPRouter
//			-. reception of report -> state change	-- IGMPRouter

//			5. filter packet	                    -- IGMPRouterFilter
//			6. reception of data -> route			-- IGMPRouterState

// TODO RFC-5.1: change of interface state

// 1. compare before after -> report with records
// 2. merge reports
// 3. merged stops retransmissions of state-change for the same multicast adresses
// 4. send merged qrv times

// new change before retransmissions done -> merge state-change report instead
// merge report is new TO_IN of TO_EX in our case?

// TODO RFC-5.2: reception of query

// on query: schedule response in ]0, MaxRespTime[ or merge if existing
// general interface timer, per-group timer
// 1. sooner response to general -> no
// 2. general -> schedule + cancel
// 3. group + no pending -> schedule
// 4. group + pending -> schedule min(pending, new)

// on general timer expiration:
// send report with all records

// on group timer expiration:
// send report with one record if exists

class IGMPClient: public Element {
public:
	const char* class_name() const override { return "IGMPClient"; }
	const char* port_count() const override { return "1/1"; }
	const char* processing() const override { return PUSH; }

	int  configure(Vector<String>&, ErrorHandler*) override;
	void add_handlers() override;

	void push(int, Packet*) override;

	static int handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh);
	static int handleLeave(const String& conf, Element* e, void* thunk, ErrorHandler* errh);

	void scheduleStateChangeMessage(RecordType type, IPAddress address);

private:
	IGMPClientState*           state;
	uint32_t                   qrv                       = 2;
	const uint32_t             unsolicitedReportInterval = 1000;
	std::unordered_map<IPAddress, Timer*, Hash> timers;

	struct ScheduledReport {
		IGMPClient* client;
		Packet*     packet;
		uint32_t    remaining;
	};

	static void handleReport(Timer* timer, void* data);
};

void printMessage(std::string front, ReportMessage* message);

CLICK_ENDDECLS
#endif    // IGMPCLIENT_H