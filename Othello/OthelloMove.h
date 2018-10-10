#ifndef __OTHELLOMOVE_H
#define __OTHELLOMOVE_H

#include <iostream>
#include <string>
#include <vector>

/*
An OthelloMove is a move in a othello game consisting of
a row and col indicating where to place the move.
*/
class OthelloMove {
private:
   // OthelloBoard is a friend so it can access mRow and mCol.
   friend class OthelloBoard;

   /*
   A FlipSet tracks a direction specified by rowDelta and colDelta
   and maintains the number of pieces that got flipped when this 
   move was applied
   */
   class FlipSet {
   public:
      FlipSet(char sw, char rowD, char colD) : flipped(sw), rowDelta(rowD), 
       colDelta(colD) {}

      char flipped;
      char rowDelta, colDelta;
   };

   // the row and column of the move
   int mRow, mCol;

   // a list of FlipSets representing the enemy pieces flipped
   std::vector<FlipSet> mFlips;

   /*
   KEEP THESE CONSTRUCTORS PRIVATE.
   Default constructor: initializes this move as a PASS.
   */
   OthelloMove();

   // Constructor: initializes an OthelloMove with the arguments row and column.
   OthelloMove(int row, int col);

   /*
   Adds a FlipSet to the move's vector of sets. Used to keep track of how many
   enemy pieces were flipped in each direction when this move was applied.
   This function is private, but OthelloBoard has friend access.
   */
   void AddFlipSet(FlipSet set) {mFlips.push_back(set);}

public:

   static int mOnHeap;

   // the following two functions were provided to assist
   // with visualization of OthelloMoves on the heap
   /*static void* operator new(std::size_t sz) {
      mOnHeap++;
      std::cout << "operator new: " << mOnHeap << " OthelloMoves on the heap" << std::endl;
      return ::operator new(sz);
   }

   static void operator delete(void* ptr, std::size_t sz) {
      mOnHeap--;
      std::cout << "operator delete: " << mOnHeap << " OthelloMoves on the heap" << std::endl;
      ::operator delete(ptr);
   }*/

   /*
   This assignment operator takes a string representation of an Othellomove
   and uses it to initialize the move. The string is in the format
   (r, c); OR is the string "pass".
   */
   OthelloMove& operator=(const std::string &move);

   // Compares two OthelloMove objects for equality (if rows and cols are same).
   friend bool operator==(const OthelloMove &lhs, const OthelloMove &rhs);

   /*
   Converts the OthelloMove into a string representation that could be
   used with operator=(string). Returns "pass" if move is a pass.
   */
   operator std::string() const;

   // Returns true if the move represents a Pass.
   inline bool IsPass() const {
      /* return true if this move is a "pass" */
      if (mRow == -1 && mCol == -1) {
         return true;
      }
      return false;
   }
};
#endif
