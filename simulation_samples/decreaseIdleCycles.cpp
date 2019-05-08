#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

int main(int argc, char* argv[]){
	double percentage = atoi(argv[1])/100.0;

	string line;
	ifstream simFile(argv[2]);

	if(simFile.is_open()){
		while(getline(simFile, line)){
			if(line.substr(0, line.find(':')) == "CR"){
				cout << line.substr(0, line.find(':')+2);
				string content = line.substr(line.find(':')+2);
				cout << content.substr(0, content.find(' ')+1);
				content = content.substr(content.find(' ')+1);
				cout << content.substr(0, content.find(' ')+1);
				content = content.substr(content.find(' ')+1);
				cout << content.substr(0, content.find(' ')+1);
				content = content.substr(content.find(' ')+1);
				cout << content.substr(0, content.find(' ')+1);
				content = content.substr(content.find(' ')+1);
				cout << content.substr(0, content.find(' ')+1);
				content = content.substr(content.find(' ')+1);

				cout << (int)(stoi(content) * percentage) << endl;
			}else{
				cout << line << endl;
			}
		}
	}

	simFile.close();

	return 0;
}