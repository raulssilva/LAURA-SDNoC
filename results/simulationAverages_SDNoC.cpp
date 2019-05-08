#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

int main(int argc, char* argv[]){
	int maxDelay = 0;
	int transCounter = 0;
	double averageDelay = 0;
	int totalSimTime = 0;

	vector< tuple<string, int> > delays;

	string line;
	ifstream simFile(argv[1]);

	if(simFile.is_open()){
		while(getline(simFile, line)){
			if(line.substr(0, line.find('_')) == "Laura"){
				string content = line.substr(line.find('_')+14);
				string srcCore = content.substr(0, content.find(' '));
				
				int vectorHasCore = -1;
				for(int i = 0; i < delays.size(); i++){
					if(get<0>(delays[i]) == srcCore){
						vectorHasCore = i;
						break;
					}
				}

				content = content.substr(content.find(' ')+1);

				if(content.substr(0, content.find(' ')) == "Requesting"){
					content = content.substr(content.find(':')+2);
					int cycles = stoi(content.substr(0, content.find(' ')));

					if(vectorHasCore >= 0){
						delays[vectorHasCore] = make_tuple(get<0>(delays[vectorHasCore]), cycles);
					}else{
						delays.push_back(make_tuple(srcCore, cycles));
					}
				}else if(content.substr(0, content.find(' ')) == "Request"){
					content = content.substr(content.find(':')+1);
					int cycles = stoi(content.substr(0, content.find(' ')));

					if(vectorHasCore >= 0){
						int delay = cycles - get<1>(delays[vectorHasCore]);
						transCounter++;
						if(delay > maxDelay){
							maxDelay = delay;
						}
						
						averageDelay += delay;
					}
				}
			}else if(line.substr(0, line.find(' ')) == "Ending"){
				string content = line.substr(line.find(':')+2);
				totalSimTime = stoi(content.substr(0, content.find(' ')));
			}
		}
	}

	simFile.close();

	averageDelay = averageDelay/transCounter;

	cout << "Avrg: " << averageDelay << endl;
	cout << "MaxDelay: " << maxDelay << endl;
	cout << "TotalCycles: " << totalSimTime << endl;

	return 0;
}