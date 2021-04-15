#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "fixIPDest.hh"
#include "IGMPMessages.hh"

CLICK_DECLS
FixIPDest::FixIPDest() = default;

FixIPDest::~FixIPDest() = default;

int FixIPDest::configure(Vector<String>& conf, ErrorHandler* errh) { return 0; }

void FixIPDest::push(int, Packet* p) {
	auto packet = p->uniqueify();

	auto ip    = (click_ip*) (packet->data());
	auto query = (QueryMessage*) (packet->data() + packet->ip_header_length());
	auto dest  = query->groupAddress;

	if (!dest.s_addr) return output(0).push(p);

	ip->ip_dst = dest;

	ip->ip_sum = 0;
	ip->ip_sum = click_in_cksum((unsigned char*) (ip), int(packet->ip_header_length()));

	packet->set_dst_ip_anno(dest);
	output(0).push(packet);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FixIPDest)