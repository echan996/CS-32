#ifndef HISTORY_
#define HISTORY_
#include "globals.h"
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int list[MAXROWS][MAXCOLS], rows, cols;
};
#endif