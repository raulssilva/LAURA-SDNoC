#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
	double averageSimTime = 0;
	int initialTime = 1002;

	string dir = argv[2];

	for(int i = 1; i <= atoi(argv[1]); i++){
		string statsFileName = dir + to_string(i) + ".txt";

		string line;
		ifstream simFile(statsFileName);

		if(simFile.is_open()){
			while(getline(simFile, line)){
				if(line.substr(0, line.find(':')) == "TIME"){
					string content = line.substr(line.find(':')+2);
					averageSimTime += stoi(content.substr(0, content.find(' '))) - initialTime;
					break;
				}
			}
		}

		simFile.close();

	}

	averageSimTime = averageSimTime/atoi(argv[1]);
	cout << "TotalCycles: " << averageSimTime << endl;

	return 0;
}