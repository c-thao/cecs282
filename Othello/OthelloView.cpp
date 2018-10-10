#include "OthelloView.h"
#include <iostream>

using namespace std;

// Prints the gameboard from an OthelloBoard
void OthelloView::PrintBoard(ostream &s) const {
	s << "\n   0 1 2 3 4 5 6 7" << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << " " << i << " ";
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (mOthelloBoard->mBoard[i][j] == 0) {
				cout << '.'; // if empty print a .
			}
			else if (mOthelloBoard->mBoard[i][j] == 1) {
				cout << 'B'; // if the player is 1, then print B for BLACK
			}
			else if (mOthelloBoard->mBoard[i][j] == -1) {
				cout << 'W'; // if the player is -1, then print W for WHITE
			}
			else {
				cout << '?';
			}
			cout << " ";
		}
		cout << endl;
	}
}

// overload the << operator to print the game board to a stream
ostream& operator<< (std::ostream &lhs, const OthelloView &rhs) {
	rhs.PrintBoard(lhs);
	return lhs;
}
