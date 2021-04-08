#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
#include "IGMPRouter.hh"

CLICK_DECLS
int IGMPRouter::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
            .read_mp("STATE", ElementCastArg("IGMPRouterState"), state)
            .complete()) {
        return errh->error("Could not parse IGMPRouterState");
    }

    return 0;
}

void IGMPRouter::push(int input, Packet *packet) {
    auto report = (ReportMessage *) (packet->data() + packet->ip_header_length());

    // Idk if this actually doesn't happen, just for safety
    if (input < 0) return;

    processReport(report, static_cast<uint32_t>(input));
}

void IGMPRouter::processReport(ReportMessage *report, uint32_t interface) {
    if (report->type != REPORT) return;

    if (state->interfaces.find(interface) == state->interfaces.end()) {
        state->interfaces.emplace(interface, Groups{});
    }

    for (auto i = 0; i < report->NumGroupRecords; i++) {
        GroupRecord *record = (GroupRecord *) (report + sizeof(ReportMessage)) + i;

        // what if doesn't exist?
        auto group = state->interfaces[interface][record->multicastAddress];

        // Conceptually, when a group record is received, the router filter-mode
        // for that group is updated to cover all the requested sources using
        // the least amount of state. As a rule, once a group record with a
        // filter-mode of EXCLUDE is received, the router filter-mode for that
        // group will be EXCLUDE.

        // Spoopy fallthrough weeee
        switch (record->recordType) {
            case RecordType::MODE_IS_EXCLUDE:
            case RecordType::CHANGE_TO_EXCLUDE_MODE:
                // Exclude {} -> Someone wants to listen so we set it to true
                std::get<2>(group) = true;



                // TODO
                // start group timer
                break;
            case RecordType::MODE_IS_INCLUDE:
                // Include {} -> Want to be unsubscribed
                // But the group itself will remain as EXCLUDE
                break;
            case RecordType::CHANGE_TO_INCLUDE_MODE:
                // TODO
                // A Group-Specific Query is sent to verify there are no systems that desire
                // reception of the specified group or to "rebuild" the desired
                // reception state for a particular group.  Group-Specific Queries are
                // sent when a router receives a State-Change record indicating a system
                // is leaving a group.
                break;
        }
    }
}

void IGMPRouter::sendQuery() {

}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouter)
