#include <systemc.h>
#include <iostream>
#include "constants.h"
#include "core.h"

using namespace std;

int sc_main(int argc, char* argv[]){
	cout << "------------------ BEGIN SIMULATION ------------------" << endl;
	sc_clock clk("clock", 1, SC_NS, 0.5);
	sc_signal<bool> start;
	sc_signal<bool> channel_available;
	sc_signal< sc_uint<CHANNEL_WIDITH> > data_in;
	sc_signal< sc_uint<CORE_ID> > requested_CoreId;
	sc_signal< sc_uint<CHANNEL_WIDITH> > data_out;
	sc_signal<bool> thread_finished;

	Core core("core");
	core.clk(clk);
	core.start(start);
	core.channel_available(channel_available);
	core.data_in(data_in);
	core.requested_CoreId(requested_CoreId);
	core.data_out(data_out);
	core.thread_finished(thread_finished);
	
	core.idleCycles.push_back(4);
	core.idleCycles.push_back(2);
	start = true;
	cout << thread_finished << endl;
	sc_start(10, SC_NS);
	cout << thread_finished << endl;
	


	// sc_stop(); // Vai quando todo mundo acabar de enviar os pacotes

	cout << "--------------- TERMINATING SIMULATION ---------------" << endl;
	return 0;
}