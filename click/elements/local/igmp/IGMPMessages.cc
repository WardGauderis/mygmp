#include <ip.h>
#include "IGMPMessages.hh"

uint32_t QueryMessage::maxRespTime() const {
	if (maxRespCode < 128) return maxRespCode;
	const uint8_t mant = maxRespCode & 0x0F;
	const uint8_t exp  = (maxRespCode >> 4) & 0x07;
	return (mant | 0x10) << (exp + 3);
}

bool QueryMessage::check() const {
	// TODO ref-4.1.10: include source addresses and additional IP octets?
	return !click_in_cksum((const unsigned char*) this, sizeof(*this));
}

uint32_t QueryMessage::QQI() const {
	if (qqic < 128) return qqic;
	const uint8_t mant = qqic & 0x0F;
	const uint8_t exp  = (qqic >> 4) & 0x07;
	return (mant | 0x10) << (exp + 3);
}
