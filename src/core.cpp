#include "core.h"

void Core::requestRoute(){
	finish.write(0);
	requested_coreX.write(-1);
	requested_coreY.write(-1);
	while(true){
		wait();
		if(start.read() == 1){
			if(!idleCycles.empty()){
				if(idleCycles.front() <= (sc_time_stamp() - cyclesElapsed).value()/1000){
					cout << "Requesting route at: " << sc_time_stamp() << endl;
					requested_coreX.write(get<0>(destinyCores.front()));
					requested_coreY.write(get<1>(destinyCores.front()));

					while(channel_available.read() != 1){
						wait();
					}
					requested_coreX.write(-1);
					requested_coreY.write(-1);

					sendPackages();

					idleCycles.erase(idleCycles.begin());
				}
			}else{
				finish.write(1);
			}
		}

		if(last_pckgReceived == 1){
			last_pckgReceived.write(false);
		}
	}
}

void Core::sendPackages(){
	for(int i = 0; i < numPckgs.front(); i++){
		wait();
		cout << "Send: " << packages[i] << " At:" << sc_time_stamp() << endl;
		data_out.write(packages[i]);
	}

	for(int i = 0; i < numPckgs.front(); i++){
		packages.erase(packages.begin());
	}

	numPckgs.erase(numPckgs.begin());
}

void Core::receivePackages(){
	last_pckgReceived.write(false);
	if(data_in.read() != 4294967295 && data_in.read() != 0){
		cout << "Receiving: " << data_in.read() << " At:" << sc_time_stamp() << endl;
	}
	if(data_in.read() == 1){
		cyclesElapsed = sc_time_stamp();
		last_pckgReceived.write(true);
	}
}
















