//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_IGMP_H
#define CLICK_IGMP_H

#include <element.hh>
CLICK_DECLS
class IGMP: public Element {
public:
	const char* class_name() const override { return "IGMP"; }
	const char* port_count() const override { return "1/1"; }
	const char* processing() const override { return AGNOSTIC; }

	int  configure(Vector<String>&, ErrorHandler*) override;
	void add_handlers() override;

	int handleJoin(const String& conf, Element* e, void* thunk, ErrorHandler* errh);
	int handleLeave(const String& conf, Element* e, void* thunk, ErrorHandler* errh);

	void    push(int, Packet*) override;
	Packet* pull(int) override;

private:
};

CLICK_ENDDECLS
#endif    // CLICK_IGMP_H