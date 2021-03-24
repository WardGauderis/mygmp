//
// Created by thomas on 24/03/21.
//

#ifndef CLICK_IGMPROUTERSTATE_H
#define CLICK_IGMPROUTERSTATE_H


class IGMPRouterState {
    const char* class_name() const override { return "IGMPRouterState"; }
    const char* port_count() const override { return "0"; }
};


#endif //CLICK_IGMPROUTERSTATE_H
