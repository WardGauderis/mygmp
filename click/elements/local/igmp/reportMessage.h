//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_REPORTMESSAGE_H
#define CLICK_REPORTMESSAGE_H

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
	MODE_IS_EXCLUDE = 2,

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

struct GroupRecord {
	// 4.2.5. Record Type
	uint8_t recordType;

	// 4.2.6. Aux Data Len
	uint8_t auxDataLen;

	// 4.2.7. Number of Sources (N)
	uint16_t numSources;

	// 4.2.8. Multicast Address
	uint32_t multicastAddress;

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
	uint8_t type;

	// 4.2.1. Reserved
	uint8_t reserved;

	// 4.2.2. Checksum
	uint16_t checksum;

	// 4.2.1. Reserved
	uint16_t reserved2;

	// 4.2.3. Number of Group Records (M)
	uint16_t NumGroupRecords;
};

#endif    // CLICK_REPORTMESSAGE_H
