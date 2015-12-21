#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	vector<string> storage;
	if (hostIn.size() == 0)
		return false;
	string add;
	for (int i = 0; i < hostIn.size(); i++){	//push everything up to the newline char
		if (hostIn[i] != '\n')
			add += hostIn[i];
		else{
			storage.push_back(add);
			add.clear();
		}
	}
	if (!add.empty()){
		storage.push_back(add);
		add.clear();
	}
	for (int i = 0; i < storage.size(); i++){		//pop everything off that is a return or a tab or a space
		if (storage[i].size() == 0)
			continue;
		if (storage[i].back() == '\r')
			storage[i].pop_back();
		for (int a = storage[i].size()-1; a >= 0; a--)
			if (storage[i][a] == ' ' || storage[i][a] == '\t')
				storage[i].pop_back();
			else
				break;
	}
	vector<unsigned short> comp;
	Compressor::compress(msg, comp);
	string addOn = BinaryConverter::encode(comp);
	int i = 0;
	int addOnSpot = 0;
	for (; i < addOn.size() % storage.size(); i++){		//append the message to the end of the vector
		for (int b = 0; b < (addOn.size() / storage.size()) + 1; b++, addOnSpot++){
			storage[i].push_back(addOn[addOnSpot]);
		}
	}	
	for (; i < storage.size(); i++)
		for (int b = 0; b < addOn.size() / storage.size(); b++, addOnSpot++){
		storage[i].push_back(addOn[addOnSpot]);
		}
	
	hostOut.clear();								//add newline chars
	for (int a = 0; a < storage.size(); a++){
		hostOut += storage[a];
		hostOut += '\n';
	}
	
	return true;
	// This compiles, but may not be correct
}


bool Steg::reveal(const string& hostIn, string& msg) 
{
	vector<string> a;
	string add;
	for (int i = 0; i < hostIn.size(); i++){		//add everything up to the newline char
		if (hostIn[i] != '\n'){
			add += hostIn[i];
		}
		else{
			a.push_back(add);
			add.clear();
		}
	}
	if (!add.empty())
		a.push_back(add);
	int count = 0, lC=0;
	string decomp;
	string addon;

	for (int i = 0; i < a.size(); i++)
		for (int b = a[i].size() - 1; b > -1; b--){
		if (b == 0 && (a[i][b] == ' ' || a[i][b]=='\t'))
			decomp += a[i];
		else if (a[i][b] != ' ' && a[i][b] != '\t'){		//add all the spaces and tabs using substrings
			decomp += a[i].substr(b+1, a[i].size() - 1);
			break;
		}
		
		}
	vector<unsigned short> num;
	if (!BinaryConverter::decode(decomp, num))
		return false;
	string output;
	if (!Compressor::decompress(num, output))
		return false;
	msg = output;
	return true;

}