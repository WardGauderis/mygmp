//#ifndef IGMPROUTERSTATE_HH
//#define IGMPROUTERSTATE_HH
//
//#include <click/element.hh>
//#include <tuple>
//
//// pair = (source address, source timer)
//using Source = std::pair<IPAddress, Timer*>;
//
//// tuple = (group address, group timer, sources, is include)
//using Group = std::tuple<IPAddress, Timer*, Vector<Source>, bool>;
//
//// the index of the group is the interface.
//using Interface = Vector<Group>;
//
//CLICK_DECLS
//class IGMPRouterState : public Element {
//public:
//    const char* class_name() const override { return "IGMPRouterState"; }
//    const char* port_count() const override { return "0"; }
//
//    Group* getGroup(uint32_t interface, IPAddress address);
//    void resizeToFit(uint32_t interface);
//
//    Vector<Interface> interfaces;
//};
//CLICK_ENDDECLS
//
//#endif //IGMPROUTERSTATE_HH