#include <systemc.h>
#include <iostream>
#include "constants.h"
#include "network.h"
#include "manager.h"

using namespace std;

int sc_main(int argc, char* argv[]){
	
	sc_clock clk("clock", 1, SC_NS, 0.5);

	sc_signal<int> signal_RouteRequests[N*M];
	sc_signal<bool> signal_RoutingAlgorithm;
	sc_signal<bool> signal_AvailableChannels[N*M];
	sc_signal< sc_uint<ROUTERS_SWITCHERS> > signal_SwtBitsteam[N][M];
	sc_signal< sc_uint<ROUTERS_ENABLES> > signal_enBitstream[N][M];

	Network laura("Laura_Network");
	laura.clk(clk);

	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			laura.switches_bitstream[i][j](signal_SwtBitsteam[i][j]);
			laura.enables_bitstream[i][j](signal_enBitstream[i][j]);
		}
	}

	Manager manager("Manager");
	manager.clk(clk);

	for(int i = 0; i < N*M; i++){
		manager.route_requests[i](signal_RouteRequests[i]);
		manager.available_channels[i](signal_AvailableChannels[i]);
	}

	manager.routing_algorithm(signal_RoutingAlgorithm);

	// Setting signals before the simulation
	switch(atoi(argv[1])){
		case 0:
			signal_RoutingAlgorithm = DIJKSTRA;
			break;
		case 1:
			signal_RoutingAlgorithm = XY;
			break;
		default:
			cout << endl;
			cout << "***************** ERROR *****************" << endl;
			cout << "Available routing algortihms:" << endl;
			cout << "    0 - Dijskstra" << endl;
			cout << "    1 - XY" << endl;
			cout << "*****************************************" << endl;
			cout << endl;
			return -1;
	}

	// cout << argv[1] << endl;

	cout << "------------------ BEGIN SIMULATION ------------------" << endl;










	// sc_signal<int> route_requests[N*M];
	// sc_signal<bool> routing_algorithm;
	// sc_signal<bool> channels_available[N*M];

	// Manager manager("manager");
	// manager.clk(clk);
	// for(int i = 0; i < N*M; i++){
	// 	manager.route_requests[i](route_requests[i]);
	// 	route_requests[i] = -1;

	// 	manager.channels_available[i](channels_available[i]);
	// }
	// manager.routing_algorithm(routing_algorithm);

	// routing_algorithm = XY;
	// route_requests[1].write(3);
	// sc_start(10, SC_NS);






	// sc_signal<bool> start;
	// sc_signal<bool> channel_available;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > data_in;
	// sc_signal<int> requested_CoreId;
	// sc_signal< sc_uint<CHANNEL_WIDITH> > data_out;
	// sc_signal<bool> thread_finished;

	// Core core("core");
	// core.clk(clk);
	// core.start(start);
	// core.channel_available(channel_available);
	// core.data_in(data_in);
	// core.requested_CoreId(requested_CoreId);
	// core.data_out(data_out);
	// core.thread_finished(thread_finished);
	
	// core.idleCycles.push_back(4);
	// core.idleCycles.push_back(2);
	// start = true;
	// cout << thread_finished << endl;
	// sc_start(10, SC_NS);
	// cout << thread_finished << endl;
	


	// sc_stop(); // Vai quando todo mundo acabar de enviar os pacotes

	cout << "--------------- TERMINATING SIMULATION ---------------" << endl;
	return 0;
}