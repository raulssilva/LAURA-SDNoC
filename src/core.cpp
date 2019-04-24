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
		data_out.write(packages[i]);
	}

	for(int i = 0; i < numPckgs.front(); i++){
		packages.erase(packages.begin());
	}

	numPckgs.erase(numPckgs.begin());
}

void Core::receivePackages(){
	last_pckgReceived.write(false);
	cout << data_in.read() << endl;
	if(data_in.read() == 1){
		cyclesElapsed = sc_time_stamp();
		last_pckgReceived.write(true);
	}
}
















