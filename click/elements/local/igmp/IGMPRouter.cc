#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
#include "IGMPRouter.hh"

CLICK_DECLS
int IGMPRouter::configure(Vector<String>& conf, ErrorHandler* errh) {
//    if (Args(conf, this, errh)
//            .read_mp("STATE", ElementCastArg("IGMPRouterState"), state)
//            .complete()) {
//        return errh->error("Could not parse IGMPRouterState");
//    }

    return 0;
}

void IGMPRouter::push(int input, Packet* packet)
{
    auto report = (ReportMessage*)(packet->data() + packet->ip_header_length());

    // Idk if this actually doesn't happen, just for safety
    if(input >= 0)
    {
        processReport(report, static_cast<uint32_t>(input));
    }
}

void IGMPRouter::processReport(ReportMessage* report, uint32_t interface) {
//    if(report->type != REPORT)
//    {
//        return;
//    }
//
//    state->resizeToFit(interface);
//
//    for (auto i = 0; i < report->NumGroupRecords; i++)
//    {
//        GroupRecord* record = (GroupRecord*)(report + sizeof(ReportMessage)) + i;
//
//        auto group = state->getGroup(interface, record->multicastAddress);
//
//        // Conceptually, when a group record is received, the router filter-mode
//        // for that group is updated to cover all the requested sources using
//        // the least amount of state. As a rule, once a group record with a
//        // filter-mode of EXCLUDE is received, the router filter-mode for that
//        // group will be EXCLUDE.
//        switch (record->recordType)
//        {
//            case RecordType::MODE_IS_INCLUDE:
//
//                break;
//            case RecordType::MODE_IS_EXCLUDE:
//                break;
//            case RecordType::CHANGE_TO_INCLUDE_MODE:
//                break;
//            case RecordType::CHANGE_TO_EXCLUDE_MODE:
//                break;
//        }
//
//        // TODO:  Group-Specific Queries are
//        // sent when a router receives a State-Change record indicating a system
//        // is leaving a group.
//    }
}

void IGMPRouter::sendQuery()
{

}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouter)
