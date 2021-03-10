AddressInfo(sourceAddr 10.0.0.1 1A:7C:3E:90:78:41)
AddressInfo(responderAddr 10.0.0.2 1A:7C:3E:90:78:42)

flow::FastUDPSource(10, 10000, 60, 1A:7C:3E:90:78:41, 10.0.0.1, 1234, 1A:7C:3E:90:78:42, 10.0.0.2, 1234);
source::ICMPPingSource(sourceAddr, responderAddr, INTERVAL 0.2);
responder::ICMPPingResponder;
priority::PrioSched;
switch::ListenEtherSwitch


elementclass Router { $src | 
	
	
	querier::ARPQuerier($src);
	responder::ARPResponder($src);
	arpclass::Classifier(12/0806 20/0001, 12/0806 20/0002, -); // queries, responses, data
	
	input [0]
		-> [0]querier;

	querier
		-> [0]output;

	input [1]
        // read list
        // read sourceRouter/Counter@4.count
        // "zou opzich ook in responderRouter kunnen"
        // read responderRouter/Counter@4.rate
        // die @4 kan varieren

        -> Counter
		-> arpclass;
		
	arpclass[0] // queries
		-> responder
		-> [0]output; // to network
  	
	arpclass[1] // replies
		-> [1]querier;
	
	arpclass[2] // data
		-> filter::HostEtherFilter($src);
		
	filter[0]
		-> Strip(14)
		-> MarkIPHeader
		-> [1] output

	filter[1]
		-> Discard;
}

source
    -> Counter(COUNT_CALL 5 source.interval 0.01) // change interval after 5 packets
    -> Queue    
    -> Shaper(2) // limit to 2 packets per second
	-> [0] priority
	
flow
	-> [1] priority

priority  // define a priority element between udp/icmp streams
    -> Unqueue
	-> [0] sourceRouter::Router(sourceAddr) [0]
    -> [0] switch
	
switch[0]
	-> [1] sourceRouter [1]
	-> source

responder
	-> [0] responderRouter::Router(responderAddr) [0]
	-> [1] switch
	
switch[1]
	-> [1] responderRouter [1]
	-> responder

switch[2]
	-> ToDump(switch.pcap)
	-> Discard



