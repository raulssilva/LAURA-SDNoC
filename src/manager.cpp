#include "manager.h"

void Manager::routeRequest(){
	while(true){
		wait();
		for(int i = 0; i < N*M; i++){

			if(route_requests[i].read() != -1){
				bool isOnStack = false;

				for(int j = 0; j < requests_queue.size(); j++){
					if(requests_queue[j] == i){
						isOnStack = true;
						break;
					}
				}

				if(!isOnStack){
					requests_queue.push_back(i);
				}
			}
		}
	}
}

void Manager::generateRoutes(){
	while(true){
		wait();
		if(requests_queue.size() > 0){
			if(routing_algorithm.read()){
				xy((requests_queue[0]%3), (requests_queue[0]/3), (route_requests[requests_queue[0]].read()%3), (route_requests[requests_queue[0]].read()/3));
			}else{
				// dijkstra(requests_queue[0], route_requests[requests_queue[0]].read());
			}
		}
	}
}

void Manager::xy(int srcX, int srcY, int destX, int destY){
	vector<int> path;
	path.push_back((srcX + srcY * N));

	while(srcX < destX){
		if(network[(srcX + srcY * N)][(srcX + 1 + srcY * N)] == 0){
			return;
		}

		path.push_back((srcX + 1 + srcY * N));
		srcX++;
	}

	while(srcX > destX){
		if(network[(srcX + srcY * N)][(srcX - 1 + srcY * N)] == 0){
			return;
		}

		path.push_back((srcX - 1 + srcY * N));
		srcX--;
	}

	while(srcY < destY){
		if(network[(srcX + srcY * N)][(srcX + (srcY + 1) * N)] == 0){
			return;
		}

		path.push_back((srcX + (srcY + 1) * N));
		srcY++;
	}

	while(srcY > destY){
		if(network[(srcX + srcY * N)][(srcX + (srcY - 1) * N)] == 0){
			return;
		}

		path.push_back((srcX+ (srcY - 1) * N));
		srcY--;
	}

	for(int i = 0; i < path.size() - 1; i++){
		network[path[i]][path[i+1]] = 0;
	}

	paths.push_back(path);

	requests_queue.erase(requests_queue.begin());
}

void Manager::printNetwork(){
	for(int i = 0; i < N*M; i++){
		for(int j = 0; j < N*M; j++){
			cout << network[i][j] << " ";
		}

		cout << endl;
	}
}















