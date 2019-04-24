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

					// cout << sc_time_stamp() << endl;
			// 		while(channel_available.read() != 1){
			// 			wait();
			// 		}

			// 		cyclesElapsed = sc_time_stamp();
			// 		idleCycles.erase(idleCycles.begin());
				}
			}else{
				finish.write(1);
			}
		}
	}
}