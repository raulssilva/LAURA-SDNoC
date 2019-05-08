#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

int main(int argc, char* argv[]){
	int maxDelay = 0;
	double averageDelay = 0;
	int totalSimTime = 0;

	string dir = argv[2];

	for(int i = 1; i <= atoi(argv[1]); i++){
		string statsFileName = dir + "stats_" + to_string(i) + ".txt";

		string line;
		ifstream statsFile(statsFileName);

		if(statsFile.is_open()){
			while(getline(statsFile, line)){
				if(line.substr(0, line.find(':')) == "Avrg"){
					string content = line.substr(line.find(':')+2);
					averageDelay += stod(content);
				}else if(line.substr(0, line.find(':')) == "MaxDelay"){
					string content = line.substr(line.find(':')+2);
					if(stoi(content) > maxDelay){
						maxDelay = stoi(content);
					}
				}else if(line.substr(0, line.find(':')) == "TotalCycles"){
					string content = line.substr(line.find(':')+2);
					totalSimTime += stoi(content);
				}
			}
		}

		statsFile.close();
	}

	averageDelay = averageDelay/atoi(argv[1]);
	totalSimTime = totalSimTime/atoi(argv[1]);

	cout << "Avrg Delay: " << averageDelay << endl;
	cout << "MaxDelay: " << maxDelay << endl;
	cout << "TotalCycles: " << totalSimTime << endl;

	return 0;
}