#include "OthelloBoard.h"
#include <iostream>

using namespace std;

// Default constructor initializes the board to its starting "new game" state
OthelloBoard::OthelloBoard()
	: mNextPlayer(BLACK), mValue(0), mPassCount(0) {
	// initialize board to EMPTY(0) indicating blank places on the board
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			mBoard[i][j] = EMPTY;
		}
	}

	// initialize the center with four pieces alternating with
	// BLACK(1) and WHITE(-1)
	mBoard[BOARD_SIZE / 2][BOARD_SIZE / 2] = BLACK;
	mBoard[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = BLACK;
	mBoard[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = WHITE;
	mBoard[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = WHITE;

}

/*
Fills in a vector with all possible moves on the current board state for
the current player. The moves should be ordered based first on row, then on
column. Example ordering: (0, 5) (0, 7) (1, 0) (2, 0) (2, 2) (7, 7)
If no moves are possible, then a single OthelloMove representing a Pass is
put in the vector.
Any code that calls ApplyMove is responsible for first checking that the
requested move is among the possible moves reported by this function.

Precondition: the vector is empty.
Postcondition: the vector contains all valid moves for the current player.
*/
void OthelloBoard::GetPossibleMoves(std::vector<OthelloMove *> *list) const {
	for (int i = 0; i < BOARD_SIZE; i++) { // tranverse each row
		for (int j = 0; j < BOARD_SIZE; j++) { // tranverse each column
			if (mBoard[i][j] == EMPTY) { // try to find a empty square on the board
				bool fnv = false; // found and validated a possible move
				for (int u = -1; u <= 1; u++) { // tranverse top to down
					for (int v = -1; v <= 1; v++) { // tranverse left to right
						int r = i + u; // row/horizontal
						int c = j + v; // column/vertical
						char opponent = -1 * mNextPlayer; //get opponent
						if (!InBounds(r, c)) { // if postition around is out of bound then break
							continue;
						}
						if (mBoard[r][c] != opponent) { // check if square is opponent's
							continue;
						}

						while (mBoard[r][c] == opponent) { // as long as still detecting opponent pieces
							r += u; // increment row/horizontal
							c += v; // increment column/vertical
							if (!InBounds(r, c)) { // if next turn is out of bounds then escape
								continue;
							}
							if (mBoard[r][c] == mNextPlayer) { // if met with a player piece then add it to the list
								list->emplace_back(new OthelloMove(i, j));
								fnv = true;
								break;
							}
						}
						if (fnv) break; // found and validated a possible move
					}
					if (fnv) break; // found and validated a possible move
				}
			}
		}
	}
	if (list->empty()) { // empty means there are no moves
		list->emplace_back(new OthelloMove(-1, -1)); // add a pass to the list of possible moves
	}
}

/*
Applies a valid move to the board, updating the board state accordingly.
You may assume that this move is valid, and is consistent with the list
of possible moves returned by GetAllMoves. Make sure you account for changes
to the current player, pass count, and board value.
*/
void OthelloBoard::ApplyMove(OthelloMove *move) {
	// update history with move
	mHistory.push_back(move);

	// if move is a pass, then update mNextPlayer to
	// switch to the other player's turn and increment
	// mPassCount and exit function (return)
	if (move->IsPass()) {
		mNextPlayer *= -1;
		mPassCount++;
		return;
	}

	// update the current square on the board indicated by move
	int row = move->mRow;
	int col = move->mCol;
	mBoard[row][col] = mNextPlayer; // add new player piece to mBoard
	mValue += mNextPlayer; // update mValue for new player piece
	mPassCount = 0; // resets mPassCount to 0 as move is not a pass

	bool fnv = false; // found and validated
	// update all opponent pieces between all adjacent player pieces from this move
	for (int i = -1; i <= 1; i++) { // tranverse top to down
		for (int j = -1; j <= 1; j++) { // tranverse left to right
			int r = row + i;
			int c = col + j;
			char opponent = -1 * mNextPlayer; //get opponent
			if (!InBounds(r, c)) { // if postition around is out of bound then break
				continue;
			}
			if (mBoard[r][c] != opponent) { // check if square is opponent's
				continue;
			}

			// find an adjacent player piece
			while (mBoard[r][c] == opponent) { // as long as still detecting opponent pieces
				r += i; // increment row/horizontal to check next piece
				c += j; // increment column/vertical to check next piece
				if (!InBounds(r, c)) { // if next turn is out of bounds then escape
					continue;
				}
				if (mBoard[r][c] == mNextPlayer) { // if met with a player piece then add it to the list
					fnv = true; // set found and validate to ensure opponent pieces are updated
					r -= i; // decrement row/horizontal to move to previous opponent piece
					c -= j; // decrement column/vertical to move to previous opponent piece
					break;
				}
			}

			// update all opponent pieces to player pieces
			if (fnv == true) {
				char sw = 0;
				do {
					mBoard[r][c] = mNextPlayer; // update opponent pieces
					mValue += 2 * mNextPlayer; // update mValue due to switching of pieces
					sw++; // increment the number of pieces switched
					r -= i; // decrement row/horizontal to get next previous piece
					c -= j; // decrement column/vertical to get next previous piece
				} while (r != row || c != col);
				move->AddFlipSet(OthelloMove::FlipSet(sw, i, j));
				fnv = false; // reset found and validate
			}
		}
	}
	mNextPlayer *= -1; // update mNextPlayer to the opponent
}

/*
Undoes the last move applied to the board, restoring it to the state it was
in prior to the most recent move (including whose turn it is, what the
board value is, and what the pass count is).
*/
void OthelloBoard::UndoLastMove() {
	// if no moves have happened then exit function
	if (GetMoveHistory()->size() == 0) {
		cout << "No moves found." << endl;
		return;
	}

	// get last move that had occurred
	OthelloMove* move = GetMoveHistory()->back();
	cout << "Last move was at " << string(*move) << endl;

	// check if move was a pass then update the player
	// remove the move
	if (move->IsPass()) {
		mNextPlayer *= -1; // change mNextPlayer to previous player
		delete move; // delete the move object
		mHistory.erase(mHistory.end()-1); // erase the last move from the vector
		return;
	}

	// get all flips that occurred and remove them
	for (vector<OthelloMove::FlipSet>::iterator fs = move->mFlips.begin(); fs != move->mFlips.end(); fs++) {
		char sw = fs->flipped;
		int row = move->mRow;
		int col = move->mCol;
		int i = fs->rowDelta;
		int j = fs->colDelta;
		while (sw > 0) {
			row += i;
			col += j;
			mBoard[row][col] = mNextPlayer;
			mValue += 2 * mNextPlayer;
			sw--;
		}
	}

	// empty the place where the move was applied,
	// update mValue to indicate the move removal,
	// update the player, and then remove the move
	// from mHistory
	mBoard[move->mRow][move->mCol] = EMPTY;
	mValue += mNextPlayer;
	mNextPlayer *= -1; // change mNextPlayer to previous player
	delete move; // delete the move object
	mHistory.erase(mHistory.end() - 1); // erase the last move from the vector
}
