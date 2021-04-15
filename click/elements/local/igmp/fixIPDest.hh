#ifndef CLICK_IPDESTINATIONFIXER_HH
#define CLICK_IPDESTINATIONFIXER_HH
#include <click/element.hh>
CLICK_DECLS

class FixIPDest: public Element {
public:
	FixIPDest();
	~FixIPDest() override;

	const char* class_name() const override { return "FixIPDest"; }
	const char* port_count() const override { return "1/1"; }
	const char* processing() const override { return PUSH; }
	int         configure(Vector<String>&, ErrorHandler*) override;

	void push(int, Packet*) override;
};

CLICK_ENDDECLS
#endif