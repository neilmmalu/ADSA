//HW3 Name: Neil Malu
//SUID: 635692900 NetID: nemalu
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
	int y_coord; //y_coord goes from 1-8
	char letter;  //Letter goes from 'a' to 'h'

	bool visited = false; //true if a particular position is visited
	bool inQueue = false; //true if a particular position is already in the BFS queue
	pair<char, int> parent; //parent position of current position - used for printing path

	vector<int> x_add = { -1, -2, -2, -1, 1, 2, 2, 1 }; //Order of x-axis neighbor calculation
	vector<int> y_add = { -2, -1, 1, 2, 2, 1, -1, -2 }; //Order of y-axis neighbor calculation

	pos() { pos('a', 0); } //Overrided default constructor

	//x coord is the letter
	//y coord is the number
	pos(char x, int y) {
		this->letter = x;
		this->y_coord = y;
	}

	//Utility print function
	void print() {
		cout << "(" << letter << ", " << y_coord << ")";
	}

	//Checks if pair of x, y values are valid position on the board
	bool isValid(char x, int y) {
		if (x < 'a' || x > 'h') return false;
		if (y < 1 || y > 8) return false;

		return true;
	}

	//Returns a list of all valid neighbors of a pos, sorted clockwise from bottom left to bottom right
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

	//Set up stuff for recursive DFS
	//Mark start pos as visited and add to stack
	stack<pair<int, int>> Stack_r;
	

	int x = start_pos.first - 'a';
	int y = 8 - start_pos.second;
	board[y][x].visited = true;
	Stack_r.push(start_pos);
	DFS_r(board, Stack_r, start_pos, end_pos);//recursive DFS

	return 0;
}


void BFS(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos) {

	//Push the start position to the queue
	queue<pair<char, int>> Q;
	Q.push(start_pos);
	while (!Q.empty()) {
		auto p = Q.front();
		Q.pop();

		//Calculations to convert from pair to pos on board
		int x = p.first - 'a';
		int y = 8 - p.second;

		//exit condition - if the end position is found
		if (p.first == end_pos.first && p.second == end_pos.second) {
			//Keep printing while x and y values don't correspond to the start position
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

		//If the current position is not visited
		if (!board[y][x].visited) {
			//Mark the current position as visited
			board[y][x].visited = true;
			//find all the neighbors of the current position
			vector<pair<char, int>> neighbors = board[y][x].allValidPos();
			for (auto n : neighbors) {
				int newX = n.first - 'a';
				int newY = 8 - n.second;
				//If the neighbor is not in queue or not visited
				if (!board[newY][newX].visited && !board[newY][newX].inQueue) {
					//Add the neighbor to the queue
					board[newY][newX].inQueue = true;
					//Set the parent of the neigbor as the current position
					board[newY][newX].parent = p;
					Q.push(n);
				}
			}
		}


	}
	cout << "No path available!" << endl;
}


void DFS_r(vector<vector<pos>>& board, stack<pair<int, int>>& Stack_r, pair<char, int> start_pos, pair <char, int> end_pos) {
	//Initially, start position is already in the stack
	while (!Stack_r.empty()) {
		auto p = Stack_r.top();

		//Calculations to convert from pair to pos on board
		int x = p.first - 'a';
		int y = 8 - p.second;

		//exit condition - if the end position is found
		if (p.first == end_pos.first && p.second == end_pos.second) {
			//Keep printing while x and y values don't correspond to the start position
			while (!(x == start_pos.first - 'a' && y == 8 - start_pos.second)) {
				board[y][x].print();
				cout << " <-- ";
				int newX = board[y][x].parent.first - 'a';
				int newY = 8 - board[y][x].parent.second;
				x = newX;
				y = newY;
			}
			board[y][x].print();
			Stack_r.pop();
			return;
		}

		//find all the neighbors of the current position
		vector<pair<char, int>> neighbors = board[y][x].allValidPos();
		for (auto n : neighbors) {
			int newX = n.first - 'a';
			int newY = 8 - n.second;
			//If the neighbor is not visited
			if (!board[newY][newX].visited) {
				//Mark the neighbor as visited
				board[newY][newX].visited = true;
				//Set the parent of the neighbor as the current pos
				board[newY][newX].parent = p;
				//Push the neighbor to the stack
				Stack_r.push(n);
				//Recursively call DFS
				DFS_r(board, Stack_r, start_pos, end_pos);
			}
		}
		//Pop from the stack once pos is visited
		if(!Stack_r.empty())Stack_r.pop();
	}

	int endX = end_pos.first - 'a';
	int endY = 8 - end_pos.second;

	if(!board[endY][endX].visited) cout << "No path available!" << endl;
}


void DFS_nr(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos) {

	//Push the start position to the queue	
	stack<pair<char, int>> S;
	S.push(start_pos);

	while (!S.empty()) {
		auto p = S.top();
		S.pop();

		//Calculations to convert from pair to pos on board
		int x = p.first - 'a';
		int y = 8 - p.second;

		//exit condition - if the end position is found
		if (p.first == end_pos.first && p.second == end_pos.second) {
			//Keep printing while x and y values don't correspond to the start position
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

		//If the current position is not visited
		if (!board[y][x].visited) {
			//Mark the current position as visited
			board[y][x].visited = true;
			vector<pair<char, int>> neighbors = board[y][x].allValidPos();
			//Add the neigbors to the stack in reverse order, so next valid position is on top
			for (int i = neighbors.size() - 1; i >= 0; --i) {
				auto n = neighbors[i];
				int newX = n.first - 'a';
				int newY = 8 - n.second;
				//If the neighbor is not visited
				if (!board[newY][newX].visited) {
					//Set the parent of neighbor to the current pos
					board[newY][newX].parent = p;
					//Add the neighbor to the queue
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
