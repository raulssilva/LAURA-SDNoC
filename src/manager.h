#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <systemc.h>
#include <vector>
#include "constants.h"

using namespace std;

SC_MODULE(Manager){
	// Manager inputs
	sc_in<bool> clk;
	// sc_in< sc_uint<CORES_REQUESTS> > route_requests;
	vector< sc_in<bool> > route_requests;

	// Manager outputs

	// Messages

	// Behaviors

	void dijkstra();
	void xy();

	SC_CTOR(Manager):
		clk("noc_clock")
	{
		
	}
};

#endif