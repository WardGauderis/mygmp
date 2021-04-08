#ifndef IGMPCLIENT_H
#define IGMPCLIENT_H

#include <click/element.hh>
#include "IGMPMessages.hh"
#include "IGMPClientState.hh"
#include <string>
#include <unordered_map>

CLICK_DECLS
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
	IGMPClientState* state;
	uint32_t         qrv                       = 2;
	const uint32_t   unsolicitedReportInterval = 1000;

	Timer*                                      generalTimer;
	std::unordered_map<IPAddress, Timer*, Hash> groupTimers;
	std::unordered_map<IPAddress, Timer*, Hash> changeTimers;

	struct ScheduledChangeReport {
		IGMPClient* client;
		Packet*     packet;
		uint32_t    remaining;
	};

	struct ScheduledGroupReport {
		IGMPClient* client;
		IPAddress   address;
	};

	static void handleChangeReport(Timer* timer, void* data);
	static void handleGeneralReport(Timer* timer, void* data);
	static void handleGroupReport(Timer* timer, void* data);
};

void printMessage(std::string front, ReportMessage* message);
CLICK_ENDDECLS
#endif    // IGMPCLIENT_H