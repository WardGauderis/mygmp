//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_QUERYMESSAGE_H
#define CLICK_QUERYMESSAGE_H


#include <cstdint>

struct QueryMessage {
    // Type = 0x11
    uint8_t type;

    // 4.1.1. Max Resp Code
    uint8_t maxRespCode; // uses u8 float

    // 4.1.2. Checksum
    uint16_t checksum;

    // 4.1.3. Group Address
    uint32_t groupAddress;

    // 4.1.4. Resv (Reserved)
    unsigned int resv : 4;

    // 4.1.5. S Flag (Suppress Router-Side Processing)
    unsigned int s : 1;

    // 4.1.6. QRV (Querier’s Robustness Variable)
    unsigned int qrv : 3;

    // 4.1.7. QQIC (Querier’s Query Interval Code)
    uint8_t qqic; // uses u8 float

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
};

QueryMessage createGeneralQuery()
{
    /* A "General Query" is sent by a multicast router to learn the
    complete multicast reception state of the neighboring interfaces
    (that is, the interfaces attached to the network on which the
    Query is transmitted). In a General Query, both the Group Address
    field and the Number of Sources (N) field are zero. */

    return QueryMessage{0x11, 0, 0,
                 0, 0, 0,
                 0, 0, 0};
}

QueryMessage createGroupSpecificQuery()
{
    /* A "Group-Specific Query" is sent by a multicast router to learn
    the reception state, with respect to a *single* multicast address,
    of the neighboring interfaces. In a Group-Specific Query, the
    Group Address field contains the multicast address of interest,
    and the Number of Sources (N) field contains zero. */

    return QueryMessage{0x11, 0, 0,
                        0, 0, 0,
                        0, 0, 0};
}

uint32_t u8FloatConvert(uint8_t byte)
{
    if(byte < 128)
    {
        return byte;
    }
    else
    {
        uint8_t exp = (byte & 0x70) >> 4;
        uint8_t mant = byte & 0x0F;

        return (mant | 0x10) << (exp + 3);
    }
}


#endif //CLICK_QUERYMESSAGE_H
