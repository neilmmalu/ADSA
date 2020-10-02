//HW4B
//Name: Neil Malu
//NetID: nemalu SUID: 635692900
//HW4A: Dijkstra's algorithm with HEAP
//HW4B: Use the same design for Prim's algorithm
//For each x->y edges, also create a y->x edge with the same weight
//HW4A Due: Friday Oct. 2; HW4B Due: Tuesday, Oct. 6
//Complexity: O((N+E) log N)
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include<sstream>
#include <string>
#include <assert.h>
#include <algorithm>
using namespace std;


class Node {
public:
	int cost;  //cost to a node
	int from;  //the from node to this node
	bool checked;
	int h_index; //the index in heap for this node
	Node() : cost(9999), from(-1), checked(false), h_index(-1) {}
};

void Path(vector<list<pair<int, int>>>& graph, vector<Node>& table, vector<int>& heap);//HW4A
//You can create up to two help functions for Path
void MST(vector<list<pair<int, int>>>& graph, vector<Node>& table, vector<int>& heap); //HW4B
//You can create up to two help functions for MST


int main() {

	vector<list<pair<int, int>>> graph;
	vector<Node> table;
	vector<int> heap;
	//your code to read in graph from "graph.txt"
	/*
	ifstream myFile("graph.txt");
	assert(myFile.is_open());
	string line;

	getline(myFile, line);
	int source = stoi(line);
	getline(myFile, line);
	int numNodes = stoi(line);

	//Create lists corresponding to each node
	for (int i = 0; i < numNodes; ++i) {
		list<pair<int, int>> L;
		Node N;
		//Set initial h-index for all nodes as their Node IDs
		N.h_index = i;
		//Set initial heap values to match h-index
		heap.push_back(i);
		graph.push_back(L);
		table.push_back(N);
	}

	table[source].cost = 0;
	table[source].from = source;
	swap(table[source].h_index, table[0].h_index);
	swap(heap[source], heap[0]);

	//At this point: 
	//Source node has cost = 0
	//Source node is not marked checked
	//Source node has from = source
	//Source node has h-index = 0
	//heap[0] is source

	getline(myFile, line);
	int numEdges = stoi(line);

	//Get each edge
	for (int i = 0; i < numEdges; ++i) {
		getline(myFile, line);
		istringstream ss(line);
		int start;
		int end;
		int weight;

		ss >> start >> end >> weight;
		//Add each edge to corresponding node in the graph
		graph[start].push_back(pair<int, int>(end, weight));
	}

	myFile.close();

	Path(graph, table, heap);//HW4A

	for (int i = 0; i < table.size(); ++i) {
		cout << "cost from " << source << " to " << i << " is: " << table[i].cost << "\t from is: " << table[i].from;
		if (table[i].from == -1) {
			cout << "\tNo path" << endl;
			continue;
		}
		cout << "\tPath: ";
		int curr = i;
		while (curr != source) {
			cout << curr << " <- ";
			curr = table[curr].from;
		}
		cout << source << endl;
	}
	*/
	
	ifstream myFile("graph.txt");
	assert(myFile.is_open());
	string line;

	getline(myFile, line);
	int source = stoi(line);
	getline(myFile, line);
	int numNodes = stoi(line);

	//Create lists corresponding to each node
	for (int i = 0; i < numNodes; ++i) {
		list<pair<int, int>> L;
		Node N;
		//Set initial h-index for all nodes as their Node IDs
		N.h_index = i;
		//Set initial heap values to match h-index
		heap.push_back(i);
		graph.push_back(L);
		table.push_back(N);
	}

	table[source].cost = 0;
	table[source].from = source;
	swap(table[source].h_index, table[0].h_index);
	swap(heap[source], heap[0]);

	//At this point: 
	//Source node has cost = 0
	//Source node is not marked checked
	//Source node has from = source
	//Source node has h-index = 0
	//heap[0] is source

	getline(myFile, line);
	int numEdges = stoi(line);

	//Get each edge
	for (int i = 0; i < numEdges; ++i) {
		getline(myFile, line);
		istringstream ss(line);
		int start;
		int end;
		int weight;

		ss >> start >> end >> weight;
		//Add each edge to corresponding node in the graph
		graph[start].push_back(pair<int, int>(end, weight));
		graph[end].push_back(pair<int, int>(start, weight));
	}

	myFile.close();


	MST(graph, table, heap);//HW4B
	int totalCost = 0;
	for (int i = 0; i < table.size(); ++i) {
		if (i == source) continue;
		totalCost += table[i].cost;
		cout << table[i].from << " " << i << " " << table[i].cost << endl;
	}
	cout << "Overall cost: " << totalCost << endl;
	
	
}

void updateDown(vector<int>& heap, vector<Node>& table) {
	for (int i = 0; i < heap.size();) {
		int currNode = heap[i];
		int parentCost = table[currNode].cost;

		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left >= heap.size()) return;

		//At this point, current node is not a leaf node
		int l_child = heap[left];
		int minChild = left;
		if (right < heap.size()) {
			int r_child = heap[right];
			int l_cost = table[l_child].cost;
			int r_cost = table[r_child].cost;
			if (r_cost < l_cost) minChild = right;
		}

		//At this point, minChild points to the child node with the least cost (either left or right)
		int childNode = heap[minChild];
		//childNode points to the value of the child node (either l_child or r_child)
		int childCost = table[childNode].cost;

		//If current cost is lower than both children then min heap is complete
		if (parentCost <= childCost) return;

		//Otherwise, swap the parent node with the child node
		swap(heap[i], heap[minChild]);
		//Swap the h-index of the nodes in the table
		swap(table[childNode].h_index, table[currNode].h_index);

		//set the next i to the index of the parent
		i = minChild;

	}
}

void updateUp(vector<int>& heap, vector<Node>& table, int startIndex) {
	int i = startIndex;
	while (i > 0) {
		int currNode = heap[i];
		int parentIndex = (i - 1) / 2;
		if (parentIndex < 0) return;

		//At this point we know that currNode is not the root
		int parentNode = heap[parentIndex];
		int parentCost = table[parentNode].cost;

		int currCost = table[currNode].cost;

		//If the curr cost is more than the parent cost, then the min heap is complete so just return
		if (currCost >= parentCost) return;

		//At this point, the curr cost is less than parent cost so we need to swap
		swap(heap[i], heap[parentIndex]);
		swap(table[parentNode].h_index, table[currNode].h_index);

		//Then we set the curr Node to the index of the parent
		i = parentIndex;
	}
}

void Path(vector<list<pair<int, int>>>& graph, vector<Node>& Table, vector<int>& heap) {

	while (!heap.empty()) {
		//Get the node with the min cost
		int currNode = heap.front();
		if (!Table[currNode].checked) {
			//Set the current node as checked
			Table[currNode].checked = true;

			//In the heap, move the current node to the end, and delete it
			swap(heap.front(), heap.back());
			swap(Table[currNode].h_index, Table[heap.front()].h_index);
			heap.pop_back();

			updateDown(heap, Table);
			auto L = graph[currNode];
			auto N = Table[currNode];

			for (auto edge : L) {
				int end = edge.first;
				int cost = edge.second;

				if (Table[end].checked) continue;

				//At this point, this neighbor is not visited 
				int prevCost = Table[end].cost;
				int costToCurr = N.cost;

				//If cost reduces through this edge, add to path
				if (costToCurr + cost < prevCost) {
					Table[end].cost = costToCurr + cost;
					Table[end].from = currNode;
					updateUp(heap, Table, Table[end].h_index);
				}

			}
		}
	}
}

void printPath(vector<Node>& table, int curr, int source) {
	if (curr == source) {
		cout << curr << endl;
		return;
	}

	int from = table[curr].from;
	cout << curr << " <- ";
	printPath(table, from, source);
}

void MST(vector<list<pair<int, int>>>& graph, vector<Node>& table, vector<int>& heap) {

	while (!heap.empty()) {
		//Get the node with the min cost
		int currNode = heap.front();
		if (!table[currNode].checked) {
			//Set the current node as checked
			table[currNode].checked = true;

			//In the heap, move the current node to the end, and delete it
			swap(heap.front(), heap.back());
			swap(table[currNode].h_index, table[heap.front()].h_index);
			heap.pop_back();

			updateDown(heap, table);
			auto L = graph[currNode];

			for (auto edge : L) {
				int end = edge.first;
				int cost = edge.second;

				if (table[end].checked) continue;

				//At this point, this neighbor is not visited so it won't be a cycle
				int prevCost = table[end].cost;

				//If cost reduces through this edge, add to path
				if (cost < prevCost) {
					table[end].cost = cost;
					table[end].from = currNode;
					updateUp(heap, table, table[end].h_index);
				}

			}
		}
	}
}

/*
The following screenshot might not be a correct run.  It shows the required output format.

//sample screenshot for Dijkstra's algorithm
cost from 0 to 0 is : 0  from node is 0  Path: 0
cost from 0 to 1 is : 7  from node is 2  Path: 1<-2<-0
cost from 0 to 2 is : 4  from node is 0  Path: 2<-0
cost from 0 to 3 is : 10  from node is 1  Path: 3<-1<-2<-0
cost from 0 to 4 is : 14  from node is 3  Path: 4<-3<-1<-2<-0
cost from 0 to 5 is : 9  from node is 1  Path: 5<-1<-2<-0
*/

/*
//sample input file graph.txt
0 //source node
6 //# of nodes in graph
12 //# of edges in graph
0 5 16//an edge from node 0 to node 5 with a weight (cost) of 16
0 4 15
0 2 4
0 3 12
0 1 9
1 3 3
1 5 2
2 3 14
2 1 3
3 4 4
4 5 1
5 3 2
*/


/*
//sample screenshot for Prim's algorithm; 1 2 3 is the same as 2 1 3
1 2 3
2 0 4
3 5 2
4 5 1
5 1 2
The overall cost is 12.
*/
