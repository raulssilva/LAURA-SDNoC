#include "core.h"

void Core::requestRoute(){
	thread_finished.write(0);
	while(true){
		wait();
		if(start.read() == 1){
			if(!idleCycles.empty()){
				if(idleCycles.front() <= (sc_time_stamp() - cyclesElapsed).value()/1000){
					cout << sc_time_stamp() << endl;
					cyclesElapsed = sc_time_stamp();
					idleCycles.erase(idleCycles.begin());
				}
			}else{
				thread_finished.write(1);
			}
		}
	}
}