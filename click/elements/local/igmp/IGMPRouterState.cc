#include <click/config.h>
#include "IGMPRouterState.hh"
#include <click/timer.hh>

CLICK_DECLS

void IGMPRouterState::resetGroupExpire(uint32_t interface, IPAddress group) {
    if (interfaces.find(interface) == interfaces.end()) return;
    if (interfaces[interface].find(group) == interfaces[interface].end()) return;

    auto &timer = std::get<1>(interfaces[interface][group]);

    // TODO: correct timing
    timer->reschedule_after_msec(100);
}

void IGMPRouterState::groupExpire(Timer *timer, void *data) {
    auto state = (std::pair<IGMPRouterState *, bool> *) data;
    state->second = false;

    // TODO: correct timing
    timer->schedule_after_msec(100);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouterState)


