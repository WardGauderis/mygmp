//#include <click/config.h>
//#include <click/args.hh>
//#include <click/error.hh>
//#include <click/timer.hh>
//#include <clicknet/ether.h>
//#include "IGMPClient.hh"
//
//CLICK_DECLS
//int IGMPClient::configure(Vector<String>& conf, ErrorHandler* errh) {
//    if (Args(conf, this, errh)
//            .read_mp("STATE", ElementCastArg("IGMPRouterState"), state)
//            .complete()) {
//        return errh->error("Could not parse IGMPRouterState");
//    }
//
//    return 0;
//}
//
//void IGMPClient::push(int, Packet* packet)
//{
//    packet->ip_header()->ip_src;
//
//    ReportMessage* report = (ReportMessage*)(packet->data() + packet->ip_header_length());
//
//    processReport(report);
//}
//
//void IGMPClient::processReport(ReportMessage* report) {
//    if(report->type != REPORT)
//    {
//        return;
//    }
//
//    for (auto i = 0; i < report->NumGroupRecords; i++)
//    {
//        GroupRecord* record = (GroupRecord*)(report + sizeof(ReportMessage)) + i;
//
//        switch (record->recordType)
//        {
//            case RecordType::MODE_IS_INCLUDE:
//                record->multicastAddress
//                break;
//            case RecordType::MODE_IS_EXCLUDE:
//                break;
//            case RecordType::CHANGE_TO_INCLUDE_MODE:
//                break;
//            case RecordType::CHANGE_TO_EXCLUDE_MODE:
//                break;
//        }
//    }
//}
//
//CLICK_ENDDECLS
//EXPORT_ELEMENT(IGMPClient)
