#ifndef CLICK_IGMPROUTERSTATE_HH
#define CLICK_IGMPROUTERSTATE_HH

#include <click/element.hh>
#include "IGMPClientState.hh"

#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <vector>

// source address -> source timer
// using Sources = std::unordered_map<IPAddress, Timer*, Hash>;
// we can simplify this by not storing the sources and only keeping a timer

struct GroupData {
	Timer* groupTimer;
	Timer* sendTimer;
	bool   isExclude;
};

constexpr bool DEBUG = true;

// group address -> state
using Groups = std::unordered_map<IPAddress, GroupData, Hash>;

// interface id -> group
using Interfaces = std::unordered_map<uint32_t, Groups>;

CLICK_DECLS
class IGMPRouterState: public Element {
public:
	const char* class_name() const override { return "IGMPRouterState"; }

	const char* port_count() const override { return "0"; }

	Interfaces interfaces;

	// The Robustness Variable allows tuning for the expected packet loss on a network.
	// IGMP is robust to (Robustness Variable - 1) packet losses.
	// The Robustness Variable MUST NOT be zero, and SHOULD NOT be one.
	// Default: 2
	uint32_t robustness = 2;

	// The Query Interval is the interval between General Queries sent by the Querier.
	// Default: 1250 (125 seconds)
	uint32_t queryInterval = DEBUG ? 60 : 1250;

	// Query Response Interval
	// The Max Response Time used to calculate the Max Resp Code inserted into the periodic General
	// Queries. Default: 100 (10 seconds)
	uint32_t queryResponseInterval = DEBUG ? 5 : 100;

	// The Group Membership Interval is the amount of time that must pass
	// before a multicast router decides there are no more members of a
	// group or a particular source on a network.
	// This value MUST be ((the Robustness Variable) times (the Query Interval)) plus (one Query
	// Response Interval).
	uint32_t groupMembershipInterval = robustness * queryInterval + queryResponseInterval;

	// The Startup Query Interval is the interval between General Queries
	// sent by a Querier on startup.  Default: 1/4 the Query Interval.
	uint32_t startupQueryInterval = queryInterval >> 2;

	// The Startup Query Count is the number of Queries sent out on startup,
	// separated by the Startup Query Interval.
	// Default: the Robustness Variable.
	uint32_t startupQueryCount = robustness;

	// The Last Member Query Interval is the Max Response Time used to
	// calculate the Max Resp Code inserted into Group-Specific Queries sent
	// in response to Leave Group messages. It is also the Max Response
	// Time used in calculating the Max Resp Code for Group-and-Source-
	// Specific Query messages. Default: 10 (1 second)
	uint32_t lastMemberQueryInterval = 10;    // DANGER: this uses the u8-float

	// The Last Member Query Count is the number of Group-Specific Queries
	// sent before the router assumes there are no local members.  The Last
	// Member Query Count is also the number of Group-and-Source-Specific
	// Queries sent before the router assumes there are no listeners for a
	// particular source.  Default: the Robustness Variable.
	uint32_t lastMemberQueryCount = robustness;

	// The Last Member Query Time is the time value represented by the Last
	// Member Query Interval, multiplied by the Last Member Query Count.
	uint32_t lastMemberQueryTime = lastMemberQueryInterval * lastMemberQueryCount;
};

CLICK_ENDDECLS

#endif    // CLICK_IGMPROUTERSTATE_HH