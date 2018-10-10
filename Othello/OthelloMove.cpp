#include "OthelloMove.h"
#include "OthelloExceptions.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// initialize mOnHeap to start count of moves in memory
int OthelloMove::mOnHeap = 0;

/*
KEEP THESE CONSTRUCTORS PRIVATE.
Default constructor: initializes an OthelloMove as a PASS.
*/
OthelloMove::OthelloMove()
	: mRow(-1), mCol(-1) {}

// Constructor: initializes an OthelloMove with the arguments row and column.
OthelloMove::OthelloMove(int row, int col)
	: mRow(row), mCol(col) {}

/*
This assignment operator takes a string representation of an Othellomove
and uses it to initialize the move. The string is in the format
(r, c); OR is the string "pass".
*/
OthelloMove& OthelloMove::operator=(const std::string &move) {
	if (move == "pass") { // setup move as a pass
		mRow = -1;
		mCol = -1;
	}
	else { // setup move's mRow and mCol
		istringstream s(move);
		char dump;
		s >> dump >> mRow >> dump >> mCol >> dump;
		if (mRow < 0 || mRow > 7 || mCol < 0 || mCol > 7) {
			throw OthelloException("Move is out of Range.");
		}
		
	}
	return *this;
}

/*
Compares two OthelloMove objects for equality if
both their rows and cols are same then return true
else return false
*/
bool operator==(const OthelloMove &lhs, const OthelloMove &rhs) {
	return (lhs.mRow == rhs.mRow && lhs.mCol == rhs.mCol)
		? true : false;
}

/*
Converts an OthelloMove into a string representation, that could be
used with operator=(string). Returns "pass" if move is a pass.
*/
OthelloMove::operator std::string() const {
	if (mRow != -1 && mCol != -1) {
		ostringstream s;
		s << '(' << mRow << ',' << ' ' << mCol << ')';
		return s.str();
	}
	return "pass";
}

