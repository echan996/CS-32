#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	string st;
	for (int i = 0; i < numbers.size(); i++){
		st.append( convertNumberToBitString(numbers[i]));		
	}
	string output;
	for (int i = 0; i < st.size(); i++){		//change all the 0s and 1s into spaces and tabs
		if (st[i] == '0')
			output += ' ';
		else
			output += '\t';
	}
	return output;  // This compiles, but may not be correct
}
bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
	if (bitString.size() % 16 != 0)		//if its the wrong size, return
		return false;
	numbers.clear();
	string input;
	for (int i = 0; i < bitString.size(); i++){	//change all spaces and tabs into 0s and 1s 
		if (bitString[i] == ' ')
			input += '0';
		else
			input += '1';
	}
	string st;
	unsigned short add = 0;
	for (int i = 0; i < input.size(); i++){
		st += input[i];					//convert 16 into a number
		if (i % 16 == 15){
			if (!convertBitStringToNumber(st, add))
				return false;
			numbers.push_back(add);
			st.clear();
		}
	}
	return true;  // This compiles, but may not be correct
}
string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}

