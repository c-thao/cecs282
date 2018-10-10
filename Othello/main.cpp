#include "OthelloBoard.h"
#include "OthelloMove.h"
#include "OthelloView.h"
#include "OthelloExceptions.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <random>
#include <ctime>

using namespace std;

// function definitions
void PlayGame();
void PrintVector(vector<OthelloMove *> possMoves);
void ClearVector(vector<OthelloMove *> *possMoves);
bool CheckIfValidMove(vector<OthelloMove *> possMoves, OthelloMove move);
bool CheckIfValidInput(string userInput);

int main(int argc, char* argv[]) {
	PlayGame();
	return 0;
}

//Function to play a game of othello
void PlayGame() {

	// Initialization of variables
	OthelloBoard board; // the state of the game board
	OthelloView v(&board); // a View for outputting the board via operator<<
	string userInput; // a string to hold the user's command choice
	vector<OthelloMove *> possMoves; // a holder for possible moves
	OthelloMove *m; // a othellomove pointer
	bool quit = false; // boolean to determine whether to end the game
	bool displayBoard = true; // boolean to determine whether to display information

	cout << "Welcome to a game of Othello." << endl;

	// play the game
	while (!quit) {
		if (displayBoard) {
			// print out the game board
			cout << v << endl;

			// display the tally on the score of the game
			cout << "Current tally points of the game: " << board.GetValue() << endl;
			if (board.GetValue() > 0) {
				cout << "Black player in the lead." << endl;
			}
			else if (board.GetValue() < 0) {
				cout << "White player in the lead." << endl;
			}
			else {
				cout << "Game is tied." << endl;
			}

			// display the current player
			if (board.GetNextPlayer() == -1) cout << "\nCurrent Player is White." << endl;
			else cout << "\nCurrent Player is Black." << endl;

			// get all possible moves and display them for
			// the current player if it black's turn
			board.GetPossibleMoves(&possMoves);
			if (board.GetNextPlayer() == 1) PrintVector(possMoves);
		}

		// initialize a move and userInput
		m = board.CreateMove();
		userInput = " ";

		// get userInput on a move to play if the game is currently black's turn
		if (board.GetNextPlayer() == 1) {
			do {
				cout << "\nIf you wish to pass your turn type in 'pass'." << endl;
				cout << "If you wish to undo a previous move type in 'undo'." << endl;
				cout << "If, however, you wish to quit type in 'q'." << endl;
				cout << "Otherwise, please enter in a valid move in the following format: (r, c)" << endl;
				getline(cin, userInput);
			} while (!CheckIfValidInput(userInput));
		}
		else {
			// randomly select a OthelloMove from possMoves for the white player's turn
			srand(time(NULL)); // generate a new seed base off time
			int r = rand() % possMoves.size(); // randomly select a index within possMove
			userInput = string(*(possMoves[r])); // set userInput to the randomly OthelloMove indexed in possMove
		}

		if (userInput == "q") {
			// set quit to true
			delete m;
			quit = true;
		}
		else if (userInput == "undo") {
			// undo two previous moves to account for
			// black and white player as player is black
			// and computer is white
			cout << "\nUndoing" << endl;
			board.UndoLastMove();
			board.UndoLastMove();

			// set displayBoard to true and delete
			// all moves inside possMoves
			displayBoard = true;
			delete m;
		}
		else {
			// get the move from userInput
			*m = userInput;

			if (CheckIfValidMove(possMoves, *m)) { // check if move is valid
				// apply the move
				if (board.GetNextPlayer() == -1) cout << "White place a piece on ";
				else cout << "Black place a piece on ";
				cout << (string)(*m) << "." << endl;
				board.ApplyMove(m);

				// set displayBoard to true and delete
				// all moves inside possMoves
				displayBoard = true;
				//possMoves.clear();

				// quit is true if the game is finished
				quit = board.IsFinished();
			}
			else {
				// set displayBoard to false to not display the gameboard
				displayBoard = false;
			}
		}
		// clear out the vector of possible moves
		ClearVector(&possMoves);
	}

	cout << "\nExiting Game." << endl;
}

// print the list of possible moves to play
void PrintVector(vector<OthelloMove *> possMoves) {
	cout << "List of possible moves: " << endl;

	if (possMoves.empty()) { // if no moves then print None
		cout << " None" << endl;
		return;
	}

	// iterate through the vector, possMoves, and print out all moves
	for (vector<OthelloMove *>::iterator i = possMoves.begin(); i != possMoves.end(); i++) {
		cout << " " << string(**i) << " ";
	}
	cout << endl;
}

// Clear out the possible moves vector
void ClearVector(vector<OthelloMove *> *possMoves) {
	if (possMoves->empty()) return;


	// iterate through the vector, possMoves, and delete all moves
	for (vector<OthelloMove *>::iterator i = possMoves->begin(); i != possMoves->end(); i++) {
		delete *i;
	}

	// clear the vector erasing all elements
	possMoves->clear();
}

// check if a string input, userInput, is a valid string for a move
bool CheckIfValidInput(string userInput) {
	// if string is empty return false
	if (userInput.empty()) return false;

	// check if the userInput was a pass
	if (userInput == "pass" || userInput == "q" || userInput == "undo") return true;
	
	// look for string pattern in the format of (i,j)
	// where i and j are digits (0-9)
	regex pat("\\([0-9]\\,(\\s)?[0-9]\\)");

	// if the string input, userInput, does
	// not match the pattern, then return false
	if (!(regex_match(userInput, pat))) {
		return false;
	}
	
	char dump = ' '; // dump useless chars during string extraction
	char row = ' '; // row index 
	char col = ' '; // col index
	istringstream s(userInput); // use to extract string input, userInput
	
	// extract the row and col from userInput
	s >> dump >> row >> dump >> col >> dump;

	// check if the row and col are within range
	if (row < '0' || row > '7' || col < '0' || col > '7') {
		throw OthelloException(" Out of bounds or invalid");
		return false;
	}

	return true;
}

/*
check if an OthelloMove input, move, is valid move if it matches any of the
OthelloMove objects inside vector input, possMoves, then return true, else
return false
*/
bool CheckIfValidMove(vector<OthelloMove *> possMoves, OthelloMove move) {
	// if the move is a pass then return true
	// only valid for this program
	if (string(move) == "pass") {
		return true;
	}
	// iterate through the vector, possMoves, to see if an OthelloMove input, move, is valid
	for (vector<OthelloMove *>::iterator i = possMoves.begin(); i != possMoves.end(); i++) {
		if (**i == move) {
			return true;
		}
	}
	cout << "Move " << string(move) << " is not a valid move." << endl;
	return false;
}
