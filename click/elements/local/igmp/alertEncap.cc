#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include "alertEncap.hh"
#include "IGMPMessages.hh"

CLICK_DECLS
void AlertEncap::push(int, Packet* packet)
{
    // maybe some checks to be sure we received an ip packet?
    const auto header = packet->ip_header();
    const auto length = header->ip_hl * 4u;

    const auto option = RouterAlertOption{};
    auto new_packet = Packet::make(sizeof(click_ether) + 4, nullptr, packet->length() + sizeof(option), 0);

    // add the ip header
    memcpy(new_packet->data(), packet->data(), length);

    // add the ip option
    memcpy(new_packet->data() + length, &option, sizeof(option));

    // add the rest of the packet
    memcpy(new_packet->data() + length + sizeof(option), packet->data() + length, packet->length() - length);

    // Maybe do this instead if something breaks
    // packet->copy_annotations()

    // set annotations for new packet for click routing
    new_packet->set_ip_header((click_ip*)new_packet->data(), length + sizeof(option));
    new_packet->set_dst_ip_anno(packet->dst_ip_anno());

    // kill old packet
    packet->kill();

    // get new packet header
    auto* new_header = new_packet->ip_header();

    // this works in increments of 4, which is the size of the option
    new_header->ip_hl++;
    new_header->ip_len = htons(ntohs(new_header->ip_len) + sizeof(option));

    // we need to set the checksum as zero before calculating it
    new_header->ip_sum = 0;
    new_header->ip_sum = click_in_cksum(new_packet->data(), (int)(length + sizeof(option)));

    output(0).push(new_packet);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AlertEncap)
