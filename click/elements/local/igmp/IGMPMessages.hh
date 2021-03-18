#ifndef CLICK_IGMPMESSAGES_H
#define CLICK_IGMPMESSAGES_H

#include <cstdint>

enum RecordType : uint8_t {
	/* indicates that the interface has a
	filter mode of INCLUDE for the specified multicast
	address. The Source Address [i] fields in this Group
	Record contain the interface’s source list for the
	specified multicast address, if it is non-empty */
	MODE_IS_INCLUDE = 1,

	/*  indicates that the interface has a
	filter mode of EXCLUDE for the specified multicast
	address. The Source Address [i] fields in this Group
	Record contain the interface’s source list for the
	specified multicast address, if it is non-empty */
	X MODE_IS_EXCLUDE = 2,

	/* indicates that the interface
	has changed to INCLUDE filter mode for the specified
	multicast address. The Source Address [i] fields
	in this Group Record contain the interface’s new
	source list for the specified multicast address,
	if it is non-empty */
	CHANGE_TO_INCLUDE_MODE = 3,

	/*  indicates that the interface
	has changed to EXCLUDE filter mode for the specified
	multicast address. The Source Address [i] fields
	in this Group Record contain the interface’s new
	source list for the specified multicast address,
	if it is non-empty */
	CHANGE_TO_EXCLUDE_MODE = 4,
};

enum MessageType : uint8_t { QUERY = 0x11, REPORT = 0x22 };

struct QueryMessage {
	// Type = 0x11
	MessageType type;

	// 4.1.1. Max Resp Code
	uint8_t maxRespCode;    // uses u8 float

	// 4.1.2. Checksum
	uint16_t checksum;

	// 4.1.3. Group Address
	in_addr groupAddress;

	// 4.1.4. Resv (Reserved)
	unsigned int resv : 4;

	// 4.1.5. S Flag (Suppress Router-Side Processing)
	unsigned int s : 1;

	// 4.1.6. QRV (Querier’s Robustness Variable) (max 7)
	unsigned int qrv : 3;

	// 4.1.7. QQIC (Querier’s Query Interval Code)
	uint8_t qqic;    // uses u8 float

	// 4.1.8. Number of Sources (N)
	uint16_t numSources;

	// 4.1.9. Source Address [i]
	/* The Source Address [i] fields are a vector of n IP unicast addresses,
	where n is the value in the Number of Sources (N) field.*/

	// 4.1.10. Additional Data
	/* If the Packet Length field in the IP header of a received Query
	indicates that there are additional octets of data present, beyond
	the fields described here, IGMPv3 implementations MUST include those
	octets in the computation to verify the received IGMP Checksum, but
	MUST otherwise ignore those additional octets. When sending a Query,
	an IGMPv3 implementation MUST NOT include additional octets beyond
	the fields described here. */

	uint32_t maxRespTime() const;
	uint32_t QQI() const;
};

struct GroupRecord {
	// 4.2.5. Record Type
	RecordType recordType;

	// 4.2.6. Aux Data Len (units of 32-bit) (must be 0 and ignored)
	uint8_t auxDataLen;

	// 4.2.7. Number of Sources (N)
	uint16_t numSources;

	// 4.2.8. Multicast Address
	in_addr multicastAddress;

	// 4.2.9. Source Address [i]
	/* The Source Address [i] fields are a vector of n IP unicast addresses,
	where n is the value in this record’s Number of Sources (N) field. */

	// 4.2.10. Auxiliary Data
	/* The Auxiliary Data field, if present, contains additional information
	pertaining to this Group Record. The protocol specified in this
	document, IGMPv3, does not define any auxiliary data. Therefore,
	implementations of IGMPv3 MUST NOT include any auxiliary data (i.e.,
	MUST set the Aux Data Len field to zero) in any transmitted Group
	Record, and MUST ignore any auxiliary data present in any received
	Group Record. The semantics and internal encoding of the Auxiliary
	Data field are to be defined by any future version or extension of
	IGMP that uses this field. */
};

struct ReportMessage {
	// Type = 0x22
	MessageType type;

	// 4.2.1. Reserved
	uint8_t reserved;

	// 4.2.2. Checksum
	uint16_t checksum;

	// 4.2.1. Reserved
	uint16_t reserved2;

	// 4.2.3. Number of Group Records (M)
	uint16_t NumGroupRecords;
};

QueryMessage createGeneralQuery() {
	/* A "General Query" is sent by a multicast router to learn the
	complete multicast reception state of the neighboring interfaces
	(that is, the interfaces attached to the network on which the
	Query is transmitted). In a General Query, both the Group Address
	field and the Number of Sources (N) field are zero. */

	// TODO RFC-4.1.12: IP destination address 224.0.0.1
	return QueryMessage{ QUERY, 0, 0, 0, 0, 0, 0, 0, 0 };
}

QueryMessage createGroupSpecificQuery(in_addr groupAddress) {
	/* A "Group-Specific Query" is sent by a multicast router to learn
	the reception state, with respect to a *single* multicast address,
	of the neighboring interfaces. In a Group-Specific Query, the
	Group Address field contains the multicast address of interest,
	and the Number of Sources (N) field contains zero. */

	// TODO RFC--4.2.14: IP destination address 244.0.0.22
	return QueryMessage{ QUERY, 0, 0, groupAddress, 0, 0, 0, 0, 0 };
}

ReportMessage createReportMessage() { return ReportMessage{ REPORT, 0, 0, 0, 0 }; }

template<class T> void setChecksum(T& p, uint32_t length);

template<class T> bool checkChecksum(const T& p, uint32_t length);

uint32_t U8toU32(uint8_t byte);

#endif    // CLICK_IGMPMESSAGES_H
