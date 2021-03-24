#include <ip.h>
#include "IGMPMessages.hh"

uint32_t QueryMessage::maxRespTime() const
{
    return U8toU32(maxRespCode);
}

uint32_t QueryMessage::QQI() const
{
    return qqic ? U8toU32(qqic) : QQI_DEFAULT
}

QueryMessage createGeneralQuery() {
    /* A "General Query" is sent by a multicast router to learn the
    complete multicast reception state of the neighboring interfaces
    (that is, the interfaces attached to the network on which the
    Query is transmitted). In a General Query, both the Group Address
    field and the Number of Sources (N) field are zero. */

    return QueryMessage{ QUERY, 0, 0, 0, 0, 0, 0, 0, 0 };
}

QueryMessage createGroupSpecificQuery(in_addr groupAddress) {
    /* A "Group-Specific Query" is sent by a multicast router to learn
    the reception state, with respect to a *single* multicast address,
    of the neighboring interfaces. In a Group-Specific Query, the
    Group Address field contains the multicast address of interest,
    and the Number of Sources (N) field contains zero. */

    return QueryMessage{ QUERY, 0, 0, groupAddress, 0, 0, 0, 0, 0 };
}

ReportMessage createReportMessage() { return ReportMessage{ REPORT, 0, 0, 0, 0 }; }

GroupRecord createGroupRecord() { return GroupRecord{ CHANGE_TO_INCLUDE_MODE, 0, 0, 0 }; }

template<class T> void setChecksum(T& p, uint32_t length){
	p.checksum = click_in_cksum(&p, length);
}

template<class T> bool checkChecksum(const T& p, uint32_t length){
	return !click_in_cksum(&p, length);
}

uint32_t U8toU32(uint8_t byte) {
	if (byte < 128) {
		return byte;
	} else {
		uint8_t exp  = (byte & 0x70) >> 4;
		uint8_t mant = byte & 0x0F;

		return (mant | 0x10) << (exp + 3);
	}
}
