#include "History.h"
#include <iostream>
using namespace std;
History::History(int nRows, int nCols){
	rows = nRows;
	cols = nCols;
	for (int i = 0; i < MAXROWS; i++)
		for (int a = 0; a < MAXCOLS; a++)
			list[i][a] = 0;
}

bool History::record(int r, int c){
	if (r>0 && c>0 && r < rows && c < cols){
		list[r-1][c-1]++;
		return true;
	}
	return false;
}

void History::display() const{
	for (int i = 0; i < rows; i++){
		for (int a = 0; a < cols; a++){
			if (list[i][a] == 0)
				cout << '.';
			else if (list[i][a] < 26){
				char b = 'A' - 1 + list[i][a];
				cout << b;
			}
			else cout << 'Z';
		}
		cout << '\n';
	}
}
