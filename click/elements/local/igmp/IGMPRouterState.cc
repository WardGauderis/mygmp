//#include "IGMPRouterState.hh"
//
//CLICK_DECLS
//Group *IGMPRouterState::getGroup(uint32_t interface, IPAddress address)
//{
//    for(auto& group : interfaces[interface])
//    {
//        if(std::get<0>(group) == address)
//            return &group;
//    }
//    return nullptr;
//}
//
//
//
//void IGMPRouterState::resizeToFit(uint32_t interface)
//{
//    // kinda dumb but just keep the vector and expand if a larger interface is found
//    if(interface >= interfaces.size())
//    {
//        interfaces.resize(interface + 1);
//    }
//}
//
//CLICK_ENDDECLS
//EXPORT_ELEMENT(IGMPRouterState)
