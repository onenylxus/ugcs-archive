/* CSCI1540
*
* Assignment: 4
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <iostream>
using namespace std;

/* Constant */
const int N = 11;

/* Function prototypes */
char findBoardValue(char b[N][N], int u, int v);
int findPath(char b[N][N], int p, int su, int sv, int tu, int tv);
int formPath(char b[N][N]);
bool isActionValid(char b[N][N], int u, int v);
int letterToNumber(char s);
void printBoard(char b[N][N]);
char referPlayer(int x);
void updateBoard(char b[N][N], int p, int u, int v);

/* Find board value by position */
char findBoardValue(char b[N][N], int u, int v) {
	return b[u][v];
}

/* Find path (A* algorithm derivative) */
int findPath(char b[N][N], int p, int su, int sv, int tu, int tv) {
	/* Open and closed sets */
	int open[N * N], closed[N * N], neighbour[N  * N], fScore[N * N], gScore[N * N];

	int count = 0;
	int current, bestScore;

	/* Setup */
	for (int s = 0; s < N * N; s++) {
		open[s] = 0;
		closed[s] = findBoardValue(b, s % N, s / N) != referPlayer(p);
		neighbour[s] = 0;
		fScore[s] = N * N + 1;
		gScore[s] = N * N + 1;
	}

	open[sv * N + su] = 1;
	fScore[sv * N + su] = 0;
	gScore[sv * N + su] = 0;

	for (int s = 0; s < N * N; s++) {
		if (open[s] == 1) {
			count++;
		}
	}

	/* While open set is not empty */
	int cu, cv;
	while (count > 0) {
		/* Find lowest */
		current = N * N + 1;
		bestScore = N * N + 1;
		for (int k = 0; k < N * N; k++) {
			if (open[k] == 1 && fScore[k] < bestScore) {
				current = k;
				bestScore = fScore[k];
			}
		}

		/* Check if current is target */
		if (current == tv * N + tu) {
			return p;
		}
		cu = current % N;
		cv = current / N;

		/* Remove current from open and add to closed */
		open[current] = 0;
		closed[current] = 1;
		for (int k = 0; k < N * N; k++) {
			neighbour[k] = 0;
		}

		/* Find neighbours */
		if (cv > 0) {
			neighbour[current - N] = 1;
		}
		if (cu < N - 1 && cv > 0) {
			neighbour[current - N + 1] = 1;
		}
		if (cu > 0) {
			neighbour[current - 1] = 1;
		}
		if (cu < N - 1) {
			neighbour[current + 1] = 1;
		}
		if (cu > 0 && cv < N - 1) {
			neighbour[current + N - 1] = 1;
		}
		if (cv < N - 1) {
			neighbour[current + N] = 1;
		}

		/* Check neighbours */
		for (int k = 0; k < N * N; k++) {
			/* Ignore bad cases */
			if (neighbour[k] == 0 || closed[k] == 1) {
				continue;
			}

			/* Calculate scores */
			int goodScore = gScore[current] + 1;
			if (open[k] == 1) {
				if (goodScore < gScore[k]) {
					gScore[k] = goodScore;
				}
			} else {
				gScore[k] = goodScore;
				open[k] = 1;
			}
			int dx = k % 10 - tu > 0 ? k % 10 - tu : -(k % 10 - tu);
			int dy = k / 10 - tv > 0 ? k % 10 - tv : -(k % 10 - tv);
			fScore[k] = gScore[k] + dx + dy;
		}

		/* Recount */
		count = 0;
		for (int s = 0; s < N * N; s++) {
			if (open[s] == 1) {
				count++;
			}
		}
	}

	/* Failure */
	return 0;
}

/* Check if path is formed */
int formPath(char b[N][N]) {
	// Case 'O' (left-to-right)
	for (int s = 0; s < N; s++) {
		for (int t = 0; t < N; t++) {
			if (findBoardValue(b, 0, s) == 'O' && findBoardValue(b, 10, t) == 'O') {
				int r = findPath(b, 1, 0, s, 10, t);
				if (r != 0) {
					return r;
				}
			}
		}
	}

	// Case 'X' (top-to-bottom)
	for (int s = 0; s < N; s++) {
		for (int t = 0; t < N; t++) {
			if (findBoardValue(b, s, 0) == 'X' && findBoardValue(b, t, 10) == 'X') {
				int r = findPath(b, 2, s, 0, t, 10);
				if (r != 0) {
					return r;
				}
			}
		}
	}

	// Otherwise
	return 0;
}

/* Check if action is valid */
bool isActionValid(char b[N][N], int u, int v) {
	bool d = true;

	// Out of range (A-K, 0-10)
	if (u < 0 || u > 10 || v < 0 || v > 10) {
		d = false;
	}

	// Position not available
	if (findBoardValue(b, u, v) != '.') {
		d = false;
	}

	return d;
}

/* Convert letter (A-K) to corresponding number */
int letterToNumber(char s) {
	return (int)s - 65;
}

/* Print board */
void printBoard(char b[N][N]) {
	// Letter indicators
	cout << "  A B C D E F G H I J K\n";

	// Number rows
	for (int j = 0; j < N; j++) {
		// Number indicators
		for (int w = 0; w < j + 1; w++) {
			if (w == 10) {
				break;
			}
			cout << " ";
		}
		cout << j;

		// Board values
		for (int i = 0; i < N; i++) {
			cout << " " << findBoardValue(b, i, j);
		}
		cout << "\n";
	}
}

/* Refer player by steps */
char referPlayer(int x) {
	if (x == 1) {
		return 'O';
	} else if (x == 2) {
		return 'X';
	} else {
		return 0;
	}
}

/* Update board */
void updateBoard(char b[N][N], int p, int u, int v) {
	b[u][v] = referPlayer(p);
}

int main() {
	// Game variables
	char gameBoard[N][N];
	int gameStep = 1;
	char inputColumn;
	int inputRow;

	// Make board
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			gameBoard[i][j] = '.';
		}
	}

	// Loop
	printBoard(gameBoard);
	while (formPath(gameBoard) == 0) {
		bool isAsking = true;
		gameStep++;
		while (isAsking) {
			int k = gameStep % 2 + 1;
			cout << "Player " << referPlayer(k) << " moves: ";
			cin >> inputColumn >> inputRow;
			isAsking = false;
			if (isActionValid(gameBoard, letterToNumber(inputColumn), inputRow)) {
				updateBoard(gameBoard, k, letterToNumber(inputColumn), inputRow);
			}
			else {
				isAsking = true;
				cout << "Invalid. Try again!\n";
			}
		}
		printBoard(gameBoard);
	}

	// Show game result
	cout << "Player " << referPlayer(formPath(gameBoard)) << " wins!";

	return 0;
}
