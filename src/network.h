#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <systemc.h>
#include <string>
#include <iostream>
#include "constants.h"
#include "router.h"

using namespace std;

SC_MODULE(Network){
	// Network inputs
	sc_in<bool> clk;
	sc_in< sc_uint<ROUTERS_SWITCHERS> > switches_bitstream[N][M];
	sc_in< sc_uint<ROUTERS_ENABLES> > enables_bitstream[N][M];

	// Network modules
	Router *noc[N][M];

	SC_CTOR(Network):
		clk("noc_clock")
	{
		for(int i = 0; i < N; i++){
			for(int j = 0; j < M; j++){
				string node = (string)to_string(i).c_str() + "_" + (string)to_string(j).c_str();
				noc[i][j] = new Router(("router_" + node).c_str());

				// if(i == 0){
				// 	if(j == 0){
				// 		noc[i][j].

				// 		// router.switch_bitstream(bs);
				// 		// router.enable_bitstream(eb);
				// 		// router.clk(clk);
				// 		// router.local_out(o1);
				// 		// router.north_out(o2);
				// 		// router.south_out(o3);
				// 		// router.east_out(o4);
				// 		// router.west_out(o5);
				// 		continue;
				// 	}

				// }
			}
		}
	}
};

#endif