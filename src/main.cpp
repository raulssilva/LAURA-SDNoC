#include <systemc.h>
#include <iostream>
#include <fstream>
#include <string>
#include "constants.h"
#include "network.h"
#include "manager.h"

using namespace std;

int sc_main(int argc, char* argv[]){
	
	sc_clock clk("clock", 1, SC_NS, 0.5);

	sc_signal< sc_uint<ROUTERS_SWITCHERS> > signal_swtBitsteam[N][M];
	sc_signal< sc_uint<ROUTERS_ENABLES> > signal_enBitstream[N][M];
	sc_signal<bool> signal_startedThreads[N][M];
	sc_signal<bool> signal_availableChannels[N][M];
	sc_signal<bool> signal_endedCommunications[N][M];
	sc_signal<int> signal_requestedCoresX[N][M];
	sc_signal<int> signal_requestedCoresY[N][M];
	sc_signal<bool> signal_finishedThreads[N][M];
	sc_signal<bool> signal_routingAlgorithm;

	Network laura("Laura_Network");
	laura.clk(clk);

	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			laura.switches_bitstream[i][j](signal_swtBitsteam[i][j]);
			laura.enables_bitstream[i][j](signal_enBitstream[i][j]);
			laura.started_threads[i][j](signal_startedThreads[i][j]);
			laura.available_channels[i][j](signal_availableChannels[i][j]);
			laura.requested_coresX[i][j](signal_requestedCoresX[i][j]);
			laura.requested_coresY[i][j](signal_requestedCoresY[i][j]);
			laura.finished_threads[i][j](signal_finishedThreads[i][j]);
			laura.ended_communications[i][j](signal_endedCommunications[i][j]);
		}
	}

	Manager manager("Manager");
	manager.clk(clk);
	manager.routing_algorithm(signal_routingAlgorithm);

	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			manager.route_requestsX[i][j](signal_requestedCoresX[i][j]);
			manager.route_requestsY[i][j](signal_requestedCoresY[i][j]);
			manager.swtBitsteam[i][j](signal_swtBitsteam[i][j]);
			manager.enBitstream[i][j](signal_enBitstream[i][j]);
			manager.available_channels[i][j](signal_availableChannels[i][j]);
			manager.ended_communications[i][j](signal_endedCommunications[i][j]);
			manager.finished_threads[i][j](signal_finishedThreads[i][j]);
		}
	}

	// Setting signals before the simulation
	switch(atoi(argv[1])){
		case 0:
			signal_routingAlgorithm = DIJKSTRA;
			break;
		case 1:
			signal_routingAlgorithm = XY;
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

	string line;
	ifstream simFile(argv[2]);

	if(simFile.is_open()){
		while(getline(simFile, line)){
			if(line[0] == '#' || line[0] == '\0'){
				continue;
			}

			string opCode = line.substr(0, line.find(':'));
			string content = line.substr(line.find(':')+2);

			if(opCode == "ND"){

				int n = atoi(content.substr(0, content.find(' ')).c_str());
				int m = atoi(content.substr(content.find(' ')+1).c_str());
				
				if(n != N || m != M){
					cout << endl;
					cout << "***************** ERROR *****************" << endl;
					cout << "Network's dimensions must agree with the constants file" << endl;
					cout << "*****************************************" << endl;
					cout << endl;
					return -1;
				}

			}else if(opCode == "CR"){

				int srcX = atoi(content.substr(0, content.find(' ')).c_str());
				content = content.substr(content.find(' ')+1);
				int srcY = atoi(content.substr(0, content.find(' ')).c_str());

				content = content.substr(content.find(' ')+1);
				int destX = atoi(content.substr(0, content.find(' ')).c_str());
				content = content.substr(content.find(' ')+1);
				int destY = atoi(content.substr(0, content.find(' ')).c_str());

				content = content.substr(content.find(' ')+1);
				int numPckgs = atoi(content.substr(0, content.find(' ')).c_str());
				content = content.substr(content.find(' ')+1);
				int idleCycles = atoi(content.substr(0, content.find(' ')).c_str());

				laura.cores[srcX][srcY]->destinyCores.push_back(make_tuple(destX, destY));
				laura.cores[srcX][srcY]->numPckgs.push_back(numPckgs);
				laura.cores[srcX][srcY]->idleCycles.push_back(idleCycles);

				signal_startedThreads[srcX][srcY] = 1;
			}
		}
		simFile.close();
	}else{
		cout << endl;
		cout << "***************** ERROR *****************" << endl;
		cout << "No such simulation file" << endl;
		cout << "*****************************************" << endl;
		cout << endl;
	}

	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			
			signal_startedThreads[i][j] = 1;
			bool change;

			do {
				bool change = false;
				if(laura.cores[i][j]->idleCycles.size() == 0){
					break;
				}

				for(int k = 0; k < laura.cores[i][j]->idleCycles.size() - 1; k++){

					if(k == (laura.cores[i][j]->idleCycles.size() - 1)){
						break;
					}

					if(laura.cores[i][j]->idleCycles[k] > laura.cores[i][j]->idleCycles[k+1]){
						tuple<int, int> auxDestCore(get<0>(laura.cores[i][j]->destinyCores[k]), get<1>(laura.cores[i][j]->destinyCores[k]));
						int auxNumPckgs = laura.cores[i][j]->numPckgs[k];
						int auxIdleCycles = laura.cores[i][j]->idleCycles[k];

						laura.cores[i][j]->destinyCores[k] = laura.cores[i][j]->destinyCores[k+1];
						laura.cores[i][j]->destinyCores[k+1] = auxDestCore;

						laura.cores[i][j]->numPckgs[k] = laura.cores[i][j]->numPckgs[k+1];
						laura.cores[i][j]->numPckgs[k+1] = auxNumPckgs;

						laura.cores[i][j]->idleCycles[k] = laura.cores[i][j]->idleCycles[k+1];
						laura.cores[i][j]->idleCycles[k+1] = auxIdleCycles;

						change = true;
					}
				}
			} while(change);
		}
	}
	
	// for(int i = 0; i < N; i++){
	// 	for(int j = 0; j < M; j++){
	// 		for(int k = 0; k < laura.cores[i][j]->idleCycles.size(); k++){
	// 			cout << laura.cores[i][j]->name() << " - " << laura.cores[i][j]->idleCycles[k] << endl;
	// 		}
	// 	}
	// }


	cout << "------------------ BEGIN SIMULATION ------------------" << endl;

	// sc_start(200, SC_NS);
	sc_start();






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

	cout << "------------------- END SIMULATION -------------------" << endl;
	return 0;
}