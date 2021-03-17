//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_IGMPCLIENTELEMENT_H
#define CLICK_IGMPCLIENTELEMENT_H

#include <cstdint>

class ClientElement {
private:
	uint8_t robustness;    // max value 7
	uint8_t queryInterval;
	uint8_t maxResponseTime;
};

#endif    // CLICK_IGMPCLIENTELEMENT_H
