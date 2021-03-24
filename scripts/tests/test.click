state::IGMPClientState
require(library definitions.click)
RatedSource("data", 1, -1, true) -> client::IGMPClient(state)
	-> IPEncap(2, client21_address, 224.0.0.22, TTL 1)
    -> EtherEncap(0x0800, multicast_server_address:eth, multicast_server_address:eth)
    ->ToDump("dumpie.pcap")