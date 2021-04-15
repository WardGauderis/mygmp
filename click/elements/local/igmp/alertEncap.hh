#ifndef CLICK_ALERTENCAP_HH
#define CLICK_ALERTENCAP_HH

#include <click/element.hh>

class AlertEncap: public Element {
	const char* class_name() const override { return "AlertEncap"; }
	const char* port_count() const override { return "1/1"; }
	const char* processing() const override { return PUSH; }

	void push(int, Packet*) override;
};

#endif    // CLICK_ALERTENCAP_HH
