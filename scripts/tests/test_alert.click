require(library definitions.click)
RatedSource("blblaaaa", 1, -1, true)
-> IPEncap(client21_address, 224.0.0.22)
-> AlertEncap
-> IPPrint
-> EtherEncap(0x0800, multicast_server_address:eth, multicast_server_address:eth)
-> ToDump("dumpert.pcap")
