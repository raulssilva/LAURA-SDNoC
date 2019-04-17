#include "core.h"

void Core::requestRoute(){
	finish.write(0);
	while(true){
		wait();
		if(start.read() == 1){
			if(!idleCycles.empty()){
				if(idleCycles.front() <= (sc_time_stamp() - cyclesElapsed).value()/1000){

					while(channel_available.read() != 1){
						wait();
					}

					cyclesElapsed = sc_time_stamp();
					idleCycles.erase(idleCycles.begin());
				}
			}else{
				finish.write(1);
			}
		}
	}
}