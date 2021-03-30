#ifndef CLICK_IGMPROUTERFILTER_H
#define CLICK_IGMPROUTERFILTER_H


class IGMPRouterFilter {
    const char* class_name() const override { return "IGMPRouterFilter"; }
    const char* port_count() const override { return "1/2"; }
    const char* processing() const override { return PUSH; }
};


#endif //CLICK_IGMPROUTERFILTER_H
