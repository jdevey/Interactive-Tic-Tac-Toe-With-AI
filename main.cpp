#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

//Represents a move that may be chosen - it's position as well as its recursion depth
class Move {
	public:
		int x, y, depth;
		Move(int _x, int _y, int _depth) {
			x = _x;
			y = _y;
			depth = _depth;
		}
		bool operator<(const Move& rhs) const {
			return depth < rhs.depth;
		}
};

string interpretInt(int val) {
	if (val == -1) return " O ";
	if (val == 0) return "   ";
	if (val == 1) return " X ";
}

void printBoard(int board[][3]) {
	cout.width(20);
	cout << 1 << "   " << 2 << "   " << 3 << endl;
	cout.width(30);
	cout << right << "-------------" << endl;
	for (int i = 0; i < 3; ++i) {
		cout.width(16);
		cout << right << i + 1 << " |";
		for (int j = 0; j < 3; ++j) {
			cout << interpretInt(board[i][j]) << "|";
		}
		cout << endl;
		cout.width(30);
		cout << right << "-------------" << endl;
	}
	cout << endl;
}

//Returns 1 if win for isX after move x, y, 0 if tie, and -1 if incomplete
int winResult(int board[][3], bool isX, int x, int y) {
	int sym = isX ? 1 : -1;
	board[y][x] = isX ? 1 : -1;
	for (int i = 0; i < 3; ++i) {
		if (board[i][0] == sym && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {board[y][x] = 0; return 1;}
	}
	for (int i = 0; i < 3; ++i) {
		if (board[0][i] == sym && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {board[y][x] = 0; return 1;}
	}
	if (board[0][0] == sym && board[0][0] == board[1][1] && board[1][1] == board[2][2])  {board[y][x] = 0; return 1;}
	if (board[0][2] == sym && board[0][2] == board[1][1] && board[1][1] == board[2][0])  {board[y][x] = 0; return 1;}
	for (int i = 0; i < 3; ++i) {
		if (!board[i][0] || !board[i][1] || !board[i][2]) {board[y][x] = 0; return -1;}
		if (i == 2) {board[y][x] = 0; return 0;}
	}
}

//Determine who goes first
bool chooseFirst() {
	string choice;

	while (choice != "1" && choice != "2" && choice != "3") {
		cout << "Who would you like to go first?" << endl;
		cout << "1. Me (Enter '1')" << endl;
		cout << "2. CPU (Enter '2')" << endl;
		cout << "3. Random (Enter '3')" << endl;
		getline(cin, choice);
	}

	if (choice == "1") return true;
	if (choice == "2") return false;
	else {
		return (bool)((rand() >> 8) % 2); //Shift right for randomness
	}
}

//Do a depth-first search where the base case involves a win or draw for current team's turn
int dfs(int board[][3], vector <vector <Move>>& results, bool isX, int depth) {
	if (depth > 10) return 0; //Useful for debugging, does nothing if greater than 9.

	int draws = 0, wins = 0;

	//Examine all 9 squares using a double for-loop
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (!board[i][j]) {
				int result = winResult(board, isX, j, i);
				//Base cases
				if (result == 1) {
					if (!depth) results[2].push_back(Move(j, i, depth));
					return 1;
				}
				if (result == 0) {
					if (!depth) results[1].push_back(Move(j, i, depth));
					return 0;
				}

				board[i][j] = isX ? 1 : -1;
				result = dfs(board, results, !isX, depth + 1);
				if (result == -1) {
					if (!depth) results[2].push_back(Move(j, i, depth));
					++wins;
				}
				else if (result == 0) {
					if (!depth) results[1].push_back(Move(j, i, depth));
					++draws;
				}
				else {
					if (!depth) results[0].push_back(Move(j, i, depth));
				}
				board[i][j] = 0;
			}
		}
	}
	if (wins) return 1;
	if (draws) return 0;
	return -1;
}

//We want to win as fast as possible, and lose as slowly as possible
void filterResults(vector <Move>& choices, bool isWin) {
	//Reverse sign so that it sorts in opposite order if we are talking about a loss vector
	if (!isWin) for (int i = 0; i < choices.size(); ++i) choices[i].depth = -choices[i].depth;
	sort(choices.begin(), choices.end());
	int minDepth = choices[0].depth;
	for (int i = (int)choices.size() - 1; i > 0; --i) {
		if (choices[i].depth > minDepth) choices.erase(choices.begin() + i);
	}
}

//Use randomization so that the same move isn't always chosen
void chooseRandMove(vector <Move>& choices, bool& isX, int& x, int& y) {
	int choice = rand() % (int)choices.size();
	x = choices[choice].x;
	y = choices[choice].y;
}

void findBestMove(int board[][3], bool isX, int& x, int& y) {
	//Let results[0] represent losses, results[1] represent draws, and results[2] represent wins
	vector <vector <Move>> results(3, vector <Move>());

	dfs(board, results, isX, 0);

	if (!results[2].empty()) {filterResults(results[2], true); chooseRandMove(results[2], isX, x, y);}
	else if (!results[1].empty()) chooseRandMove(results[1], isX, x, y);
	else {filterResults(results[0], false); chooseRandMove(results[0], isX, x, y);}
}

bool isValidInput(int board[][3], string input) {
	string validOptions[] = {"1 1", "1 2", "1 3", "2 1", "2 2", "2 3", "3 1", "3 2", "3 3"};
	for (int i = 0; i < 9; ++i) {
		if (input == validOptions[i]) break;
		if (i == 8) {
			cout << "Enter your input in the form \"x y\"." << endl;
			return false;
		}
	}
	if (board[input[2] - '1'][input[0] - '1']) {
		cout << "The square you choose must be empty!" << endl;
		return false;
	}
	return true;
}

void printEndMessage(int winner) {
	if (winner == -1) cout << "You lost! better luck next time!" << endl;
	else if (winner == 0) cout << "The game is a draw!" << endl;
	else cout << "You won! Nice job!" << endl;
	cout << endl;
}

//Runs once per game
void play(bool& wantsToPlay) {

	int board[3][3] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	bool isHuman = chooseFirst(), isX = true;
	int winner = 0, moveCount = 0;

	while (moveCount < 9) {
		if (isHuman) {
			printBoard(board);
			cout << "Your turn! Choose a point to place your symbol." << endl;
			cout << "Enter your input in the form \"x y\"." << endl;
			string input;
			getline(cin, input);
			cout << endl;
			while (!isValidInput(board, input)) {
				getline(cin, input);
				cout << endl;
			}
			if (winResult(board, isX, input[0] - '1', input[2] - '1') == 1) {
				board[input[2] - '1'][input[0] - '1'] = isX ? 1 : -1;
				winner = 1;
				break;
			}
			board[input[2] - '1'][input[0] - '1'] = isX ? 1 : -1;
		}
		else {
			int x, y;
			findBestMove(board, isX, x, y);
			if (winResult(board, isX, x, y) == 1) {
				board[y][x] = isX ? 1 : -1;
				winner = -1;
				break;
			}
			board[y][x] = isX ? 1 : -1;
		}
		isX = !isX;
		isHuman = !isHuman;
		++moveCount;
	}
	printBoard(board);
	printEndMessage(winner); //Send it 1 if the player won, 0 if it was a draw, and -1 if the player lost

	string input;
	while (input != "y" && input != "n") {
		cout << endl << "Do you want to play again? (y/n)" << endl;
		getline(cin, input);
	}

	wantsToPlay = input == "y";
}

int main() {

	cout << endl << "----- Welcome to tic tac toe! -----" << endl << endl;

	srand(time(NULL));

	bool wantsToPlay = true;
	while (wantsToPlay) play(wantsToPlay);

	cout << "Thanks for playing!" << endl;

	return 0;
}
