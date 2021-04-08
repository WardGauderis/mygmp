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

using State = std::tuple<Timer *, Timer *, bool>;

// group address -> (group timer, sources, is exclude)
using Groups = std::unordered_map<IPAddress, State, Hash>;

// interface id -> group
using Interfaces = std::unordered_map<uint32_t, Groups>;

CLICK_DECLS
class IGMPRouterState : public Element {
public:
    const char *class_name() const override { return "IGMPRouterState"; }

    const char *port_count() const override { return "0"; }

    //// if timer == 0 and mode == exclude
    // No more listeners to group. If all source timers have expired then
    // delete Group Record. If there are still source record timers
    // running, switch to  INCLUDE filter-mode using those source
    // records with running timers as the INCLUDE source record state.

    void resetGroupExpire(uint32_t interface, IPAddress group);

    static void groupExpire(Timer *timer, void *data);

    Interfaces interfaces;
};

CLICK_ENDDECLS

#endif //CLICK_IGMPROUTERSTATE_HH