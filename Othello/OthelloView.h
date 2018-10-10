#ifndef __OTHELLOVIEW_H
#define __OTHELLOVIEW_H
#include "OthelloBoard.h"
#include <iostream>

/*
An OthelloView is a wrapper around an OthelloBoard and handles
requests to print the board.
*/
class OthelloView {
private:
   // The OthelloBoard pointer.
   OthelloBoard *mOthelloBoard;

   // The method which prints to a stream.
   void PrintBoard(std::ostream &s) const;

public:
   OthelloView(OthelloBoard *b) : mOthelloBoard(b) {}

   // overload the << operator to print the game board to a stream
   friend std::ostream& operator<< (std::ostream &lhs, const OthelloView &rhs);
};
#endif
