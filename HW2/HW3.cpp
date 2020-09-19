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
	int x_coord;
	int y_coord;
	char letter;
	pair<int, int> parent;

	vector<int> x_add = { -1, -2, -2, -1, 1, 2, 2, 1 };
	vector<int> y_add = { -2, -1, 1, 2, 2, 1, -1, -2 };

	bool visited = false;
	bool inQueue = false;

	pos() {
		pos(0, 0);
	}

	pos(int i, int j) {
		this->x_coord = i;
		this->y_coord = j;
		this->letter = i + 'a';
	}

	void print() {
		cout << "(" << letter << ", " << y_coord+1 << ")";
	}

	/*
	Return the next possible position from current position

	*/
	pair<int, int> nextValidPos() {
		for (int i = 0; i < x_add.size(); ++i) {
			int newX = x_coord + x_add[i];
			int newY = y_coord + y_add[i];

			if (isValidPos(newX, newY)) return pair<int, int> (newX, newY);
		}
	}

	bool isValidPos(int x, int y) {
		if (x < 0 || x > 7) return false;
		if (y < 0 || y > 7) return false;
		return true;
	}

	vector<pair<int, int>> allValidPos() {
		vector<pair<int, int>> result;
		for (int i = 0; i < x_add.size(); ++i) {
			int newX = x_coord + x_add[i];
			int newY = y_coord + y_add[i];

			if (isValidPos(newX, newY)) result.push_back(pair<int, int>(newX, newY));
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

	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < board.size(); ++j)
			board[i][j] = pos(i, j);
	}

	/*for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < board.size(); ++j)
			board[i][j].print();
		cout << endl;
	}*/

	pair<char, int> start_pos, end_pos;
	cout << "Enter start position --  x is in a ... h and y is in 1 ... 8" << endl;
	cin >> start_pos.first >> start_pos.second;

	cout << "Enter end position -- x is in a ... h and y is in 1 ... 8" << endl;
	cin >> end_pos.first >> end_pos.second;


	BFS(board, start_pos, end_pos);
	//reset board as you see needed
	DFS_nr(board, start_pos, end_pos);//non-recursive DFS


	stack<pair<int, int>> Stack_r;
	//reset board and some initial work if you see needed.
	DFS_r(board, Stack_r, start_pos, end_pos);//recursive DFS

	return 0;
}


void BFS(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Your code
	//Note that if you can reach the end_pos, print the path otherwise print "No path available!".
	queue<pos> Q;
	int start_x = start_pos.first - 'a';
	int start_y = start_pos.second - 1;

	int end_x = end_pos.first - 'a';
	int end_y = end_pos.second - 1;
	Q.push(board[start_x][start_y]);

	while (!Q.empty()) {
		pos curr = Q.front();
		Q.pop();
		int x = curr.x_coord;
		int y = curr.y_coord;

		if (x == end_x && y == end_y) {
			while (x != start_x && y != start_y) {
				board[x][y].print();
				cout << " <-- ";
				pair<int, int> parent = board[x][y].parent;
				x = parent.first;
				y = parent.second;
			}
			board[start_x][start_y].print();
			return;
		}

		if (!curr.visited) {
			curr.visited = true;
			vector<pair<int, int>> neighbors = curr.allValidPos();
			for (auto p : neighbors){
				if (!board[p.first][p.second].visited && !board[p.first][p.second].inQueue) {
					board[p.first][p.second].parent = pair<int, int> (curr.x_coord, curr.y_coord);
					board[p.first][p.second].inQueue = true;
					Q.push(board[p.first][p.second]);
				}
			}
		}
	}

}
void DFS_r(vector<vector<pos>>& board, stack<pair<int, int>>& Stack_r, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Your code
	//Note that if you can reach the end_pos, print the path otherwise print "No path available!".
}
void DFS_nr(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Your code
	//Note that if you can reach the end_pos, print the path otherwise print "No path available!".
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
