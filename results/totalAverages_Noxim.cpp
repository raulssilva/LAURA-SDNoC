#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
	int maxDelay = 0;
	double averageDelay = 0;

	string dir = argv[2];

	for(int i = 1; i <= atoi(argv[1]); i++){
		string statsFileName = dir + to_string(i) + ".txt";

		string line;
		ifstream simFile(statsFileName);

		if(simFile.is_open()){
			while(getline(simFile, line)){
				if(line.substr(0, line.find(':')) == "% Global average delay (cycles)"){
					averageDelay += stod(line.substr(line.find(':')+2));
				}else if(line.substr(0, line.find(':')) == "% Max delay (cycles)"){
					if(stoi(line.substr(line.find(':')+2)) > maxDelay){
						maxDelay = stoi(line.substr(line.find(':')+2));
					}
				}
			}
		}

		simFile.close();

	}

	averageDelay = averageDelay/atoi(argv[1]);

	cout << "Avrg: " << averageDelay << endl;
	cout << "MaxDelay: " << maxDelay << endl;

	return 0;
}