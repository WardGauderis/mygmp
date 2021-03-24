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
