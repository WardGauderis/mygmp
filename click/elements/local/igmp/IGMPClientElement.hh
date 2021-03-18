//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_IGMPCLIENTELEMENT_H
#define CLICK_IGMPCLIENTELEMENT_H

#include <cstdint>

// TODO RFC-5.2: reception of query

// on query: schedule response in ]0, MaxRespTime[ or merge if existing
// general interface timer, per-group timer
// 1. sooner response to general -> no
// 2. general -> schedule + cancel
// 3. group + no pending -> schedule
// 4. group + pending -> schedule min(pending, new)

// on general timer expiration:
// send report with all records

// on group timer expiration:
//send report with one record if exists

class ClientElement {
private:
	uint8_t robustness;    // max value 7
	uint8_t queryInterval;
	uint8_t maxResponseTime;
};

#endif    // CLICK_IGMPCLIENTELEMENT_H
