#include "manager.h"

void Manager::routeRequest(){
	while(true){
		wait();

		for(int i = 0; i < N; i++){
			for(int j = 0; j < M; j++){
				if(route_requestsX[i][j].read() != -1 && route_requestsY[i][j].read() != -1){
					bool isOnStack = false;

					for(int k = 0; k < requests_queue.size(); k++){
						tuple<int, int> currentRequest(i, j);
						if(requests_queue[k] == currentRequest){
							isOnStack = true;
							break;
						}
					}

					if(!isOnStack){
						requests_queue.push_back(make_tuple(i, j));
					}
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
				int srcX = get<0>(requests_queue[0]);
				int srcY = get<1>(requests_queue[0]);
				int destX = route_requestsX[srcX][srcY].read();
				int destY = route_requestsY[srcX][srcY].read();
				xy(srcX, srcY, destX, destY);
			}else{
				// dijkstra(requests_queue[0], route_requests[requests_queue[0]].read());
			}
		}
	}
}

void Manager::xy(int srcX, int srcY, int destX, int destY){
	vector< tuple<int, int> > path;
	path.push_back(make_tuple(srcX, srcY));

	while(srcX < destX){
		if(network[(srcX + srcY * N)][(srcX + 1 + srcY * N)] == 0){
			return;
		}

		path.push_back(make_tuple((srcX + 1), srcY));
		srcX++;
	}

	while(srcX > destX){
		if(network[(srcX + srcY * N)][(srcX - 1 + srcY * N)] == 0){
			return;
		}

		path.push_back(make_tuple((srcX - 1), srcY));
		srcX--;
	}

	while(srcY < destY){
		if(network[(srcX + srcY * N)][(srcX + (srcY + 1) * N)] == 0){
			return;
		}

		path.push_back(make_tuple(srcX, (srcY + 1)));
		srcY++;
	}

	while(srcY > destY){
		if(network[(srcX + srcY * N)][(srcX + (srcY - 1) * N)] == 0){
			return;
		}

		path.push_back(make_tuple(srcX, (srcY - 1)));
		srcY--;
	}

	for(int i = 0; i < path.size() - 1; i++){
		int xValue = get<0>(path[i]) + get<1>(path[i]) * N;
		int yValue = get<0>(path[i+1]) + get<1>(path[i+1]) * N;
		network[xValue][yValue] = 0;
	}

	paths.push_back(path);
	requests_queue.erase(requests_queue.begin());
}

void Manager::enableRoutes(){
	while(true){
		wait();
		for(int i = 0; i < paths.size(); i++){
			vector< tuple<int, int> > path = paths[i];
			for(int j = 0; j < (path.size() - 1); j++){
				int currX = get<0>(path[j]);
				int currY = get<1>(path[j]);

				int nextX = get<0>(path[j+1]);
				int nextY = get<1>(path[j+1]);

				if(j == 0){
					if(nextX > currX){
						enables[currX][currY] = enables[currX][currY] | 4; // 00100
						switchers[currX][currY] = switchers[currX][currY] | 16; // 00 00 01 00 00
						
						swtBitsteam[currX][currY].write(switchers[currX][currY]);
						enBitstream[currX][currY].write(enables[currX][currY]);
					}else if(nextX < currX){
						enables[currX][currY] = enables[currX][currY] | 8; // 01000
						switchers[currX][currY] = switchers[currX][currY] | 0; // 00 00 00 00 00
						
						swtBitsteam[currX][currY].write(switchers[currX][currY]);
						enBitstream[currX][currY].write(enables[currX][currY]);
					}

					if(nextY > currY){
						enables[currX][currY] = enables[currX][currY] | 2; // 00010
						switchers[currX][currY] = switchers[currX][currY] | 8; // 00 00 00 10 00
						
						swtBitsteam[currX][currY].write(switchers[currX][currY]);
						enBitstream[currX][currY].write(enables[currX][currY]);
					}else if(nextY < currY){
						enables[currX][currY] = enables[currX][currY] | 1; // 00001
						switchers[currX][currY] = switchers[currX][currY] | 3; // 00 00 00 00 11
						
						swtBitsteam[currX][currY].write(switchers[currX][currY]);
						enBitstream[currX][currY].write(enables[currX][currY]);
					}
				}else{
					int prevX = get<0>(path[j-1]);
					int prevY = get<1>(path[j-1]);
					if(nextX > currX){
						enables[currX][currY] = enables[currX][currY] | 4; // 00100
						enBitstream[currX][currY].write(enables[currX][currY]);

						if(prevX < currX){
							switchers[currX][currY] = switchers[currX][currY] | 0; // 00 00 00 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevY > currY){
							switchers[currX][currY] = switchers[currX][currY] | 32; // 00 00 10 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevY < currY){
							switchers[currX][currY] = switchers[currX][currY] | 48; // 00 00 11 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}
					}else if(nextX < currX){
						enables[currX][currY] = enables[currX][currY] | 8; // 01000
						enBitstream[currX][currY].write(enables[currX][currY]);

						if(prevX > currX){
							switchers[currX][currY] = switchers[currX][currY] | 64; // 00 01 00 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevY > currY){
							switchers[currX][currY] = switchers[currX][currY] | 128; // 00 10 00 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevY < currY){
							switchers[currX][currY] = switchers[currX][currY] | 192; // 00 11 00 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}
					}

					if(nextY > currY){
						enables[currX][currY] = enables[currX][currY] | 2; // 00010
						enBitstream[currX][currY].write(enables[currX][currY]);

						if(prevX < currX){
							switchers[currX][currY] = switchers[currX][currY] | 0; // 00 00 00 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevX > currX){
							switchers[currX][currY] = switchers[currX][currY] | 4; // 00 00 00 01 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevY < currY){
							switchers[currX][currY] = switchers[currX][currY] | 12; // 00 00 00 11 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}
					}else if(nextY < currY){
						enables[currX][currY] = enables[currX][currY] | 1; // 00001
						enBitstream[currX][currY].write(enables[currX][currY]);

						if(prevX < currX){
							switchers[currX][currY] = switchers[currX][currY] | 0; // 00 00 00 00 00
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevX > currX){
							switchers[currX][currY] = switchers[currX][currY] | 1; // 00 00 00 00 01
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}else if(prevY > currY){
							switchers[currX][currY] = switchers[currX][currY] | 2; // 00 00 00 00 10
							swtBitsteam[currX][currY].write(switchers[currX][currY]);
						}
					}
				}
			}

			int currX = get<0>(path[path.size() - 1]);
			int currY = get<1>(path[path.size() - 1]);
			int prevX = get<0>(path[path.size() - 2]);
			int prevY = get<1>(path[path.size() - 2]);

			enables[currX][currY] = enables[currX][currY] | 16; // 10000
			enBitstream[currX][currY].write(enables[currX][currY]);

			if(prevX < currX){
				switchers[currX][currY] = switchers[currX][currY] | 0; // 00 00 00 00 00
				swtBitsteam[currX][currY].write(switchers[currX][currY]);
			}else if(prevX > currX){
				switchers[currX][currY] = switchers[currX][currY] | 256; // 01 00 00 00 00
				swtBitsteam[currX][currY].write(switchers[currX][currY]);
			}else if(prevY < currY){
				switchers[currX][currY] = switchers[currX][currY] | 768; // 11 00 00 00 00
				swtBitsteam[currX][currY].write(switchers[currX][currY]);
			}else if(prevY < currY){
				switchers[currX][currY] = switchers[currX][currY] | 512; // 10 00 00 00 00
				swtBitsteam[currX][currY].write(switchers[currX][currY]);
			}

		}
	}
}

void Manager::printNetwork(){
	for(int i = 0; i < N*M; i++){
		for(int j = 0; j < N*M; j++){
			cout << network[i][j] << " ";
		}

		cout << endl;
	}
}















