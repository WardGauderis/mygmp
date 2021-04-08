#include "IGMPMessages.hh"


uint32_t QueryMessage::QQI() const { return qqic ? U8toU32(qqic) : QQI_DEFAULT; }
