/* CSCI1540
*
* Assignment: 4
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <iostream>
using namespace std;

/* Function prototypes. DO NOT MODIFY! */
int status(int board, int pos);
void printBoard(int board);
void updateBoard(int& board, int from, int to, int p);
int formLine(int board);

/* Extra functions. */
int pow10(int p);
bool compare(int board, int p, int q);
bool isActionValid(int board, int player, int i, int j);
bool isMoveValid(int board, int i, int j);

/* Returns the status of position pos of the game board. DO NOT MODIFY! */
int status(int board, int pos) {
	for (int i = 0; i < 9 - pos; i++)
		board /= 10;
	return board % 10;
}

/* Returns value of powers of 10 */
int pow10(int p) {
	int v = 1;
	for (int i = 0; i < p; i++) {
		v *= 10;
	}
	return v;
}

/* Compare the status of two positions of the game board */
bool compare(int board, int p, int q) {
	return status(board, p) == status(board, q);
}

/* Prints the board to the screen. DO NOT MODIFY! */
void printBoard(int board) {
	cout << status(board, 1) << "-" << status(board, 2) << "-" << status(board, 3) << endl;
	cout << "|\\|/|\n";
	cout << status(board, 4) << "-" << status(board, 5) << "-" << status(board, 6) << endl;
	cout << "|/|\\|\n";
	cout << status(board, 7) << "-" << status(board, 8) << "-" << status(board, 9) << endl;
}

/* Check if action is valid */
bool isActionValid(int board, int player, int i, int j) {
	bool b = true;

	// Out of range (1-9)
	if (i < 1 || i > 9 || j < 1 || j > 9) {
		b = false;
	}

	// From position does not belong to player
	if (status(board, i) != player) {
		b = false;
	}

	// Improper movement
	if (!isMoveValid(board, i, j)) {
		b = false;
	}

	return b;
}

/* Check if movement is valid */
bool isMoveValid(int board, int i, int j) {
	bool b = true;
	int iCol = (i - 1) % 3 + 1;
	int iRow = (i + 2) / 3;
	int jCol = (j - 1) % 3 + 1;
	int jRow = (j + 2) / 3;
	int dCol = jCol - iCol;
	int dRow = jRow - iRow;

	// Adjust
	if (dCol < 0) {
		dCol = -dCol;
	}
	if (dRow < 0) {
		dRow = -dRow;
	}

	// Determine
	if (dCol > 1 || dRow > 1) {
		b = false;
	}
	if ((i != 5 && j != 5) && dCol > 0 && dRow > 0) {
		b = false;
	}
	if (status(board, j) != 0) {
		b = false;
	}

	return b;
}

/* Updates the from-th digit of board to 0, and the to-th digit of board to
	 digit p. The other digits of board remain unchanged.  */
void updateBoard(int& board, int from, int to, int p) {
	// TO DO below

	board += p * (-pow10(9 - from) + pow10(9 - to));
}

/* Checks if any player has formed a line horizontally, vertically, or
	 diagonally in board except his/her home row. */
int formLine(int board) {
	// TO DO below. Remember to return something.

	// Vertical
	for (int i = 0; i < 3; i++) {
		if (compare(board, i, i + 3) && compare(board, i, i + 6)) {
			return status(board, i);
		}
	}

	// Horizontal
	for (int j = 1; j < 9; j += 3) {
		if (compare(board, j, j + 1) && compare(board, j, j + 2)) {
			// Case 1
			if (status(board, j) == 1 && j != 1) {
				return 1;
			}

			// Case 2
			if (status(board, j) == 2 && j != 7) {
				return 2;
			}
		}
	}

	// Diagonal
	for (int k = 1; k < 3; k++) {
		if (compare(board, 5, 5 - 2 * k) && compare(board, 5, 5 + 2 * k)) {
			return status(board, 5);
		}
	}

	// Otherwise
	return 0;
}

int main() {
	// TO DO. Fill in the program flow below.

	// Game variables
	int gameBoard = 111000222;
	int gameStep = 1;
	int inputFrom;
	int inputTo;

	// Loop
	printBoard(gameBoard);
	while (formLine(gameBoard) == 0) {
		bool isAsking = true;
		gameStep++;
		while (isAsking) {
			cout << "Player " << gameStep % 2 + 1 << "(from to): ";
			cin >> inputFrom >> inputTo;
			isAsking = false;
			if (isActionValid(gameBoard, gameStep % 2 + 1, inputFrom, inputTo)) {
				updateBoard(gameBoard, inputFrom, inputTo, gameStep % 2 + 1);
			} else {
				isAsking = true;
				cout << "Invalid. Try again!\n";
			}
		}
		printBoard(gameBoard);
	}

	// Show game result
	cout << "Player " << formLine(gameBoard) << " wins!";

	return 0;
}
