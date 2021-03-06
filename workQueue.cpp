#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <fstream>
using namespace std;

bool checkEmpty(vector<string> &workflow){
	if (workflow.size() == 0){
		cerr<<"Workflow is empty"<<endl;
		return true;
	}
	return false;
}

int main(int argc, char *argv[]){
	if (argc < 2){
		cerr<<"Bad command-line arguments.  Format: './<executable> <fileName>/all'"<<endl;
		exit(1);
	}
	vector<string> workflow;
	vector<string> lists;
	ifstream indexFile, queueFile;
	ofstream writeFile;
	string fileName, line, line2;
	fileName = argv[1];
	if(fileName == "all"){
		indexFile.open("index.txt");
		while(getline(indexFile, line)){
			cout<<line<<":\n";
			queueFile.open(line.c_str());
			int i = 0;
			while(getline(queueFile, line2)){
				cout<<i<<"\t"<<line2<<endl;
				i++;
			}
			queueFile.close();
		}
		indexFile.close();
		exit(0);
	}
	if(fileName == "index.txt"){
		cerr<<"Bad file name.  Cannot use 'index.txt' to store list data"<<endl;
		exit(1);
	}
	bool foundList = false;
	indexFile.open("index.txt");
	while(getline(indexFile, line)){
		lists.push_back(line);
		if(line == fileName) foundList = true;
	}
	indexFile.close();
	if(!foundList){
		lists.push_back(fileName);
		writeFile.open("index.txt");
		for(int i = 0; i < lists.size(); i++) writeFile << lists[i]+"\n";
		writeFile.close();
	}
	queueFile.open(fileName.c_str());
	while(getline(queueFile,line)){
		workflow.push_back(line);
	}
	queueFile.close();
	vector<string> input;
	string temp, curList;
	bool found;
	cout<<"Welcome to your lists.  Type 'help' for help.\n";
	while(1){
		input.clear();
		cout<<"-_- % ";
		getline(cin, line);
		for (int i = 0; i < 2; i++){
			if (i == 0){
				if (line.find_first_of(" \t") == -1){
					input.push_back(line);
					line.clear();
					break;
				}
				temp = line;
				input.push_back(temp.erase(temp.find_first_of(" ")));
				line = line.substr(line.find_first_of(" "));
				line = line.substr(line.find_first_not_of(" "));
				continue;
			}
			input.push_back(line);
		}
		if (input.size() == 1){
			if (input[0] == "list" || input[0] == "ls" || input[0] == "l"){
				for (int i = 0; i < workflow.size(); i++){
					cout<<i<<"\t"<<workflow[i]<<endl;
				}
				continue;
			}
			else if (input[0] == "add" || input[0] == "a"){
				cerr<<"Form: add [value]\n";
				continue;
			}
			else if (input[0] == "remove" || input[0] == "r"){
				cerr<<"Form: remove [value/indexVal]\n";
				continue;
			}
			else if (input[0] == "all"){
				cerr<<"Form: all lists/elements\n";
				continue;
			}
			else if (input[0] == "randomize" || input[0] == "rand"){
				if (checkEmpty(workflow)) continue;
				int switchVal;
				for(int  i = 0; i < workflow.size(); i++){
					switchVal = rand()%workflow.size();
					temp = workflow[switchVal];
					workflow[switchVal] = workflow[i];
					workflow[i] = temp;
				}
				continue;
			}
			else if (input[0] == "save" || input[0] == "s"){
				cout<<"Writing to file."<<endl;
				writeFile.open(fileName.c_str());
				for(int i = 0; i < workflow.size(); i++){
					writeFile << workflow[i] + "\n";
				}
				writeFile.close();
				continue;
			}
			else if (input[0] == "help" || input[0] == "h"){
				cout<<"  COMMANDS:\n"
					<<"\tCMD Line - show all elements of all lists:\t ./<executable> all"
					<<"\tShow all lists:\t all lists\n"
					<<"\tShow all elements of all lists (while keeping focus on current list):\t all elements\n"
					<<"\tShow elements of current list:\t list/ls/l\n"
					<<"\tAdd element:\t add/a <someElement>\n"
					<<"\tRemove element:\t remove/r <someElement/indexVal>\n"
					//<<"\tNew list:\t new/n (<listName>)\n"
					//<<"\tChange list:\t change/c (<listName>)\n"
					<<"\tSave to file:\t save/s\n"
					<<"\tRandomize:\t randomize/rand\n"
					<<"\tClear:\t\t clear/c\n"
					<<"\tShow all lists:\t\tall\n"
					<<"\tQuit:\t\t quit/q"<<endl;
				continue;
			}
			else if (input[0] == "clear" || input[0] == "c"){
				cout<<"Are you sure you want to clear all elements (y/n)? ";
				getline(cin, temp);
				if (temp == "y"){
					cout<<"Clearing queue\n";
					workflow.clear();
					continue;
				}
				else if (temp == "n"){
					cout<<"Clear cancelled\n";
					continue;
				}
				else{
					cout<<"Unrecognized input.  Clear cancelled.\n";
					continue;
				}
			}
			else if (input[0] == "quit" || input[0] == "q"){
				cout<<"Are you sure you want to quit (y/n)? ";
				getline(cin, temp);
				if (temp == "y"){
					cout<<"Do you want to save your list (y/n)? ";
					temp.clear();
					getline(cin, temp);
					if (temp == "y"){
						cout<<"Writing to file."<<endl;
						writeFile.open(fileName.c_str());
						for(int i = 0; i < workflow.size(); i++){
							writeFile << workflow[i]+"\n";
						}
						writeFile.close();
					}
					cout<<"Quitting\n";
					exit(0);
				}
				else if (temp == "n"){
					cout<<"Quit cancelled\n";
					continue;
				}
				else{
					cout<<"Unrecognized input.  Quit cancelled.\n";
					continue;
				}
			}
			else if (input[0] == "terminate" || input[0] == "tm"){
				writeFile.close();
				exit(0);
			}
			else{
				cerr<<"Command not recognized\n";
				continue;
			}
		}
		else if (input.size() > 1){
			if (input[0] == "list" || input[0] == "ls"){
				if (input[1] == "all"){
					for (int i = 0; i < workflow.size(); i++){
						cout<<i<<"\t"<<workflow[i]<<endl;
					}
				}
				else if (input[1] == "top"){
					cout<<workflow[0]<<endl;
					continue;
				}
				else{
					if (input[1] == "0") cout<<workflow[0]<<endl;
					else{
						int indexValue = atoi(input[1].c_str());
						if (!indexValue){
							cerr<<"Error: incorrect parameter value\n";
							continue;
						}
						else{
							cout<<workflow[indexValue]<<endl;
							continue;
						}
					}
				}
			}
			else if (input[0] == "add" || input[0] == "a"){
				workflow.push_back(input[1]);
				continue;
			}
			else if (input[0] == "remove" || input[0] == "r"){
				if (input[1] == "0"){
					workflow.erase(workflow.begin());
					found = true;
				}
				int indexValue = atoi(input[1].c_str());
				if (!indexValue){
					for (int i = 0; i < workflow.size(); i++){
						if (input[1] == workflow[i]){
							found = true;
							workflow.erase(workflow.begin() + i);
							break;
						}
					}
					if (found) continue;
					cout<<"Workflow element not found.\n";
					continue;
				}
				else{
					if (indexValue < 0 || indexValue >= workflow.size()){
						cerr<<"Index out of range.  Try again.\n";
						continue;
					}
					workflow.erase(workflow.begin() + indexValue);
					continue;
				}
			}
			else if (input[0] == "all"){
				if(input[1] == "lists"){
					cout<<"All list files:\n";
					for(int i = 0; i < lists.size(); i++){
						cout<<lists[i]<<endl;
					}
					continue;
				}
				else if(input[1] == "elements"){
					for(int i = 0; i < lists.size(); i++){
						cout<<lists[i]<<endl;
						queueFile.open(lists[i].c_str());
						int j = 0;
						while(getline(queueFile, line)){
							cout<<j<<"\t"<<line<<endl;
							j++;
						}
						queueFile.close();
					}
					continue;
				}
				else{
					cerr<<"Parameter not recognized.  Use 'help' or 'h' for options.\n";
					continue;
				}
			}
			else{
				cerr<<"Command not recognized.  Use 'help' or 'h' for options.\n";
				continue;
			}
		}
		else{
			if (input.size() == 0) continue;
			else{
				cerr<<"Too many arguments."<<endl;
				continue;
			}
		}
	}
	return 0;
}