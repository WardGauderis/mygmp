// Router with three interfaces
// The input/output configuration is as follows:
//
// Input:
//	[0]: packets received on the 192.168.1.0/24 network
//	[1]: packets received on the 192.168.2.0/24 network
//	[2]: packets received on the 192.168.3.0/24 network
//
// Output:
//	[0]: packets sent to the 192.168.1.0/24 network
//	[1]: packets sent to the 192.168.2.0/24 network
//	[2]: packets sent to the 192.168.3.0/24 network
//  [3]: packets destined for the router itself


elementclass Router {

	$server_address, $client1_address, $client2_address |

	// Shared IP input path and routing table
	ip :: Strip(14)
		-> checker :: CheckIPHeader
		-> rt :: StaticIPLookup(
					$server_address:ip/32 0,
					$client1_address:ip/32 0,
					$client2_address:ip/32 0,
					$server_address:ipnet 1,
					$client1_address:ipnet 2,
					$client2_address:ipnet 3,
					224.0.0.0/4 4);

	// ARP responses are copied to each ARPQuerier and the host.
	arpt :: Tee (3);

	// Input and output paths for interface 0
	input[0]
		-> HostEtherFilter($server_address)
		// requests, replies, other
		-> server_class :: Classifier(12/0806 20/0001, 12/0806 20/0002, -)
		-> ARPResponder($server_address)
		-> [0]output;

	server_arpq :: ARPQuerier($server_address) -> output;
	server_class[1] -> arpt[0] -> [1]server_arpq;
	server_class[2] -> Paint(1) -> ip;


	// Input and output paths for interface 1
	input[1]
		-> HostEtherFilter($client1_address)
		-> client1_class :: Classifier(12/0806 20/0001, 12/0806 20/0002, -)
		-> ARPResponder($client1_address)
		-> [1]output;

	client1_arpq :: ARPQuerier($client1_address) -> [1]output;
	client1_class[1] -> arpt[1] -> [1]client1_arpq;
	client1_class[2] -> Paint(2) -> ip;


	// Input and output paths for interface 2
	input[2]
		-> HostEtherFilter($client2_address)
		-> client2_class :: Classifier(12/0806 20/0001, 12/0806 20/0002, -)
		-> ARPResponder($client2_address)
		-> [2]output;

	client2_arpq :: ARPQuerier($client2_address) -> [2]output;
	client2_class[1] -> arpt[2] -> [1]client2_arpq;
	client2_class[2] -> Paint(3) -> ip;


	// Local delivery
	rt[0] -> [3]output
	
	// Forwarding paths per interface
	rt[1]
		-> server_db :: DropBroadcasts
		-> server_paint :: PaintTee(1)
		-> server_ipgw :: IPGWOptions($server_address)
		-> server_ttl :: DecIPTTL
		-> server_fis :: FixIPSrc($server_address)
		-> server_frag :: IPFragmenter(1500)
		-> server_arpq;
	
	server_paint[1] -> ICMPError($server_address, redirect, host) -> rt;
	server_ipgw[1]  -> ICMPError($server_address, parameterproblem) -> rt;
	server_ttl[1]   -> ICMPError($server_address, timeexceeded) -> rt;
	server_frag[1]  -> ICMPError($server_address, unreachable, needfrag) -> rt;


	rt[2]
		-> client1_db :: DropBroadcasts
		-> client1_paint :: PaintTee(2)
		-> client1_ipgw :: IPGWOptions($client1_address)
		-> client1_ttl :: DecIPTTL
		-> client1_fis :: FixIPSrc($client1_address)
		-> client1_frag :: IPFragmenter(1500)
		-> client1_arpq;
	
	client1_paint[1] -> ICMPError($client1_address, redirect, host) -> rt;
	client1_ipgw[1]  -> ICMPError($client1_address, parameterproblem) -> rt;
	client1_ttl[1]   -> ICMPError($client1_address, timeexceeded) -> rt;
	client1_frag[1]  -> ICMPError($client1_address, unreachable, needfrag) -> rt;

	
	rt[3]
		-> client2_db :: DropBroadcasts
		-> client2_paint :: PaintTee(2)
		-> client2_ipgw :: IPGWOptions($client2_address)
		-> client2_ttl :: DecIPTTL
		-> client2_fis :: FixIPSrc($client2_address)
		-> client2_frag :: IPFragmenter(1500)
		-> client2_arpq;
	
	client2_paint[1] -> ICMPError($client2_address, redirect, host) -> rt;
	client2_ipgw[1]  -> ICMPError($client2_address, parameterproblem) -> rt;
	client2_ttl[1]   -> ICMPError($client2_address, timeexceeded) -> rt;
	client2_frag[1]  -> ICMPError($client2_address, unreachable, needfrag) -> rt;


	// IGMP
    state :: IGMPRouterState;

    filter :: IGMPRouterFilter(state);
    router :: IGMPRouter(state);

    rt[4]
        -> classifier::IPClassifier(ip proto 2, -)
        -> sw::PaintSwitch;

    classifier[1] -> filter;

    filter[0] -> server_db;
    filter[1] -> client1_db;
    filter[2] -> client2_db;

    sw[0] -> Discard;
    sw[1] -> [0]router;
    sw[2] -> [1]router;
    sw[3] -> [2]router;

    router[0]
        -> IPEncap(2, $server_address, 224.0.0.1, TTL 1, TOS 0xc0)
        -> AlertEncap
        -> FixIPDest
        -> server_fis

    router[1]
        -> IPEncap(2, $client1_address, 224.0.0.1, TTL 1, TOS 0xc0)
        -> AlertEncap
        -> FixIPDest
        -> client1_fis

    router[2]
        -> IPEncap(2, $client2_address, 224.0.0.1, TTL 1, TOS 0xc0)
        -> AlertEncap
        -> FixIPDest
        -> client2_fis
}
