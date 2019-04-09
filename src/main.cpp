#include <systemc.h>
#include <iostream>
#include "constants.h"
#include "network.h"

using namespace std;

int sc_main(int argc, char* argv[]){
	sc_signal<bool> clk;
	sc_signal< sc_uint<ROUTERS_SWITCHERS> > switches_bitstream[N][M];
	sc_signal< sc_uint<ROUTERS_ENABLES> > enables_bitstream[N][M];

	Network laura("laura");
	laura.clk(clk);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			laura.switches_bitstream[i][j](switches_bitstream[i][j]);
			laura.enables_bitstream[i][j](enables_bitstream[i][j]);
		}
	}
	// sc_signal< sc_uint<CHANNEL_WIDITH> > v1;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > v2;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > v3;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > v4;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > v5;

	// sc_signal< sc_uint<ROUTERS_SWITCHERS> > bs;
	// sc_signal< sc_uint<ROUTERS_ENABLES> > eb;
	
	// sc_signal< sc_uint<CHANNEL_WIDITH> > o1;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > o2;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > o3;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > o4;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > o5;

	// Router router("Router");
	// // router.local_in(v1);
	// // router.north_in(v2);
	// // router.south_in(v3);
	// // router.east_in(v4);
	// // router.west_in(v5);
	// router.switch_bitstream(bs);
	// router.enable_bitstream(eb);
	// router.clk(clk);
	// router.local_out(o1);
	// router.north_out(o2);
	// router.south_out(o3);
	// router.east_out(o4);
	// router.west_out(o5);

	// clk = 0;
	// sc_start();
	// clk = 1;
	// eb = 16;
	// bs = 0;
	// router.north_buffer.write(12);
	// sc_start();
	// clk = 0;
	// router.north_buffer.write(11);
	// sc_start();
	// clk = 1;
	return 0;
}