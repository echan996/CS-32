#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	if (s.empty())
		return;
	int size = s.size() / 2 + 512;
	if (size > 16384)
		size = 16384;
	string a;
	HashTable<string, unsigned short> h(size*2, size);
	for (int i = 0; i < 256; i++){	//add all basic chars
		a += static_cast<char>(i);	
		h.set(a, i, true);
		a.clear();
	}
	//	h.display();
	int nextFreeId = 256;
	string runSoFar;
	vector<unsigned short> v;
	for (int i = 0; i < s.size(); i++){		//hashtable algorithm 
		string expandedRun = runSoFar+s[i];
		unsigned short spotFinder;
		if (h.get(expandedRun, spotFinder)){
			runSoFar = expandedRun;
			continue;
		}
		h.get(runSoFar, spotFinder);
		v.push_back(spotFinder);
		h.touch(runSoFar);
		runSoFar.clear();
		unsigned short cv;
		string c;
		c += s[i];
		h.get(c,cv);
		v.push_back(cv);
		if (!h.isFull()){
			h.set(expandedRun, nextFreeId);
			nextFreeId++;
		}
		else{
			unsigned short p;
			h.discard(c, p);
			h.set(expandedRun, p);
		}	
	}
	if (!runSoFar.empty()){				//if theres still something in the string, push it onto the array
		unsigned short x;
		h.get(runSoFar, x);
		v.push_back(x);
	}
	v.push_back(size);
	numbers = v;	
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	if (numbers.size() == 0){
		s.clear();
		return true;
	}
	HashTable<unsigned short, string> h(numbers.back() * 2, numbers.back());	//push chars on
	for (int i = 0; i < 256; i++){
		string a;
		a += static_cast<char>(i);
		h.set(i, a,true);
	}
	int nextFreeId = 256;
	string runSoFar;
	string output;
	for (int i = 0; i < numbers.size()-1; i++){			//decompression algorithm 
		unsigned short us = numbers[i];
		string add;
		if (us <= 255){
			h.get(us, add);
			output += add;

			if (runSoFar.empty()){
				runSoFar += add;
				continue;
			}
			string expandedRun = runSoFar + add;
			if (!h.isFull()){
				h.set(nextFreeId, expandedRun);
				nextFreeId++;
			}
			else{
				unsigned short t;
				string worthless;
				h.discard(t, worthless);
				h.set(t, expandedRun);
			}
			runSoFar.clear();
		}
		else{
			if (!h.get(us, add)){
				return false;
			}
			h.touch(us);
			output += add;
			runSoFar = add;
		}
		
	}
	s = output;
	//h.display();
	return true;
}
