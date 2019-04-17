#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <systemc.h>
#include <string>
#include <iostream>
#include "constants.h"
#include "router.h"
#include "core.h"

using namespace std;

SC_MODULE(Network){
	// Network inputs
	sc_in<bool> clk;
	sc_in< sc_uint<ROUTERS_SWITCHERS> > switches_bitstream[N][M];
	sc_in< sc_uint<ROUTERS_ENABLES> > enables_bitstream[N][M];
	sc_in<bool> started_threads[N][M];
	sc_in<bool> channels_available[N][M];

	// Network outputs
	sc_out<int> requested_coresX[N][M];
	sc_out<int> requested_coresY[N][M];
	sc_out<bool> finished_threads[N][M];

	// Network modules
	Router *routers[N][M];
	Core *cores[N][M];

	SC_CTOR(Network):
		clk("routers_clock")
	{
		for(int i = 0; i < N; i++){
			for(int j = 0; j < M; j++){
				string node = (string)to_string(i).c_str() + "_" + (string)to_string(j).c_str();
				routers[i][j] = new Router(("router_" + node).c_str());
				cores[i][j] = new Core(("core_" + node).c_str());

				routers[i][j]->clk(clk);
				routers[i][j]->switch_bitstream(switches_bitstream[i][j]);
				routers[i][j]->enable_bitstream(enables_bitstream[i][j]);
				routers[i][j]->local_in(cores[i][j]->data_out);

				cores[i][j]->clk(clk);
				cores[i][j]->start(started_threads[i][j]);
				cores[i][j]->channel_available(channels_available[i][j]);
				cores[i][j]->data_in(routers[i][j]->local_out);
				cores[i][j]->requested_coreX(requested_coresX[i][j]);
				cores[i][j]->requested_coreY(requested_coresY[i][j]);
				cores[i][j]->finish(finished_threads[i][j]);
			}
		}

		for(int i = 0; i < N; i++){
			for(int j = i + 1; j < M; j++){
				if(i < (N - 1)){
					routers[i][j-1]->east_in(routers[i][j]->west_out);
					routers[i][j]->west_in(routers[i][j-1]->east_out);

					routers[i][j-1]->south_in(routers[i+1][j-1]->north_out);
					routers[i+1][j-1]->north_in(routers[i][j-1]->south_out);
				}else{
					routers[i][j-1]->east_in(routers[i][j]->west_out);
					routers[i][j]->west_in(routers[i][j-1]->east_out);
				}
			}
		}
	}
};

#endif