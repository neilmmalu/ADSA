//HW3:  Implement DFS and BFS as described in class.
//Due:  11:50PM, Wednesday, September 23.
//You should not add more functions.

#include <iostream>
#include <vector>
#include <queue>//bfs, in-queue
#include <stack>//dfs

using namespace std;

class pos {
public:
	//your code.
	//At each position, among many things, you need to decide how to move to a new position.
	int y_coord; //y_coord goes from 1-8
	char letter;  //Letter goes from 'a' to 'h'

	bool visited = false;
	bool inQueue = false;
	pair<char, int> parent;

	vector<int> x_add = { -1, -2, -2, -1, 1, 2, 2, 1 };
	vector<int> y_add = { -2, -1, 1, 2, 2, 1, -1, -2 };

	pos() { pos('a', 0); }

	pos(char x, int y) {
		this->letter = x;
		this->y_coord = y;
	}

	void print() {
		cout << "(" << letter << ", " << y_coord << ")";
	}

	bool isValid(char x, int y) {
		if (x < 'a' || x > 'h') return false;
		if (y < 1 || y > 8) return false;

		return true;
	}

	vector<pair<char, int>> allValidPos() {
		vector<pair<char, int>> result;
		for (int i = 0; i < x_add.size(); ++i) {
			char newX = letter + x_add[i];
			int newY = y_coord + y_add[i];

			if (isValid(newX, newY)) result.push_back(pair<char, int>(newX, newY));
		}
		return result;
	}


};


void BFS(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos);
void DFS_r(vector<vector<pos>>& board, stack<pair<int, int>>& Stack_r, pair<char, int> start_pos, pair <char, int> end_pos);
void DFS_nr(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos);


int main() {
	vector<vector<pos>> board(8, vector<pos>(8));// pos b[8][8];
	//initialize board as you see needed.

	//board[0][0] = pos "a8"
	for (int i = 0; i < board.size(); ++i)
		for (int j = 0; j < board.size(); ++j)
			board[i][j] = pos(j + 'a', 8 - i);

	pair<char, int> start_pos, end_pos;
	cout << "Enter start position --  x is in a ... h and y is in 1 ... 8" << endl;
	cin >> start_pos.first >> start_pos.second;

	cout << "Enter end position -- x is in a ... h and y is in 1 ... 8" << endl;
	cin >> end_pos.first >> end_pos.second;


	BFS(board, start_pos, end_pos);
	cout << endl;

	//reset board
	//board[0][0] = pos "a8"
	for (int i = 0; i < board.size(); ++i)
		for (int j = 0; j < board.size(); ++j)
			board[i][j] = pos(j + 'a', 8 - i);


	DFS_nr(board, start_pos, end_pos);//non-recursive DFS
	cout << endl;

	//reset board
	//board[0][0] = pos "a8"
	for (int i = 0; i < board.size(); ++i)
		for (int j = 0; j < board.size(); ++j)
			board[i][j] = pos(j + 'a', 8 - i);

	stack<pair<int, int>> Stack_r;
	//reset board and some initial work if you see needed.
	DFS_r(board, Stack_r, start_pos, end_pos);//recursive DFS

	return 0;
}


void BFS(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Your code
	//Note that if you can reach the end_pos, print the path otherwise print "No path available!".

	queue<pair<char, int>> Q;
	Q.push(start_pos);
	while (!Q.empty()) {
		auto p = Q.front();
		Q.pop();

		int x = p.first - 'a';
		int y = 8 - p.second;

		if (p.first == end_pos.first && p.second == end_pos.second) {
			while (!(x == start_pos.first - 'a' && y == 8 - start_pos.second)) {
				board[y][x].print();
				cout << " <-- ";
				int newX = board[y][x].parent.first - 'a';
				int newY = 8 - board[y][x].parent.second;
				x = newX;
				y = newY;
			}
			board[y][x].print();
			return;
		}

		if (!board[y][x].visited) {
			board[y][x].visited = true;
			vector<pair<char, int>> neighbors = board[y][x].allValidPos();
			for (auto n : neighbors) {
				int newX = n.first - 'a';
				int newY = 8 - n.second;
				if (!board[newY][newX].visited && !board[newY][newX].inQueue) {
					board[newY][newX].inQueue = true;
					board[newY][newX].parent = p;
					Q.push(n);
				}
			}
		}


	}
	cout << "No path available!" << endl;
}
void DFS_r(vector<vector<pos>>& board, stack<pair<int, int>>& Stack_r, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Your code
	//Note that if you can reach the end_pos, print the path otherwise print "No path available!".


}
void DFS_nr(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Your code
	//Note that if you can reach the end_pos, print the path otherwise print "No path available!".
	
	stack<pair<char, int>> S;
	S.push(start_pos);

	while (!S.empty()) {
		auto p = S.top();
		S.pop();

		int x = p.first - 'a';
		int y = 8 - p.second;

		if (p.first == end_pos.first && p.second == end_pos.second) {
			while (!(x == start_pos.first - 'a' && y == 8 - start_pos.second)) {
				board[y][x].print();
				cout << " <-- ";
				int newX = board[y][x].parent.first - 'a';
				int newY = 8 - board[y][x].parent.second;
				x = newX;
				y = newY;
			}
			board[y][x].print();
			return;
		}

		if (!board[y][x].visited) {
			board[y][x].visited = true;
			vector<pair<char, int>> neighbors = board[y][x].allValidPos();
			for (int i = neighbors.size() - 1; i >= 0; --i) {
				auto n = neighbors[i];
				int newX = n.first - 'a';
				int newY = 8 - n.second;
				if (!board[newY][newX].visited) {
					board[newY][newX].parent = p;
					S.push(n);
				}
			}
		}
	}
	cout << "No path available!" << endl;
	
}

//The following might not represent a correct run.  It shows the required output format.
/*
Enter start position --  x is in a ... h and y is in 1 ... 8
b 3
Enter end position -- x is in a ... h and y is in 1 ... 8
f 2

Target is reached!
(f,2)<-(e,4)<-(d,2)<-(b,3)

Target is reached!
(f,2)<-(d,1)<-(c,3)<-(a,2)<-(b,4)<-(a,6)<-(c,7)<-(a,8)<-(b,6)<-(c,4)<-(e,5)<-(g,6)<-(f,8)<-(h,7)<-(f,6)<-(g,8)<-(h,6)<-(f,5)<-(d,4)<-(b,3)

No path avaiable!
*/
