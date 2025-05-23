//Name: Neil Malu 
//SUID: 635692900 netID: nemalu
//CSE674 HW2
//Due: 11:59PM, Sunday, September 13
//Do not modify main function.
//Do not introduce new functions.
//In-place implementation is required.  External structures are not allowed.
//For example, you are not allowed to copy linked list values to outside, and then process the data and copy them
//back to linked list.


#include <iostream>
using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node* previous;
	Node(int i) { value = i; next = previous = nullptr; }
	Node() { next = previous = nullptr; }
};

class DoublyLinkedList {
public:
	Node* head;
	Node* tail;
	DoublyLinkedList() { head = tail = nullptr; }
	void makeRandomList(int m, int n);
	void printForward();
	void printBackward();

	//implement the following member function:
	void MergeSortIte(Node* p1, Node* p2);//Implement MergeSort iteratively. Sort a portion of a DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.
	void MergeSortRec(Node* p1, Node* p2); //Implement MergeSort recursively. Sort a portion of DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.
	void Merge(Node* p1, Node* p2, Node* p3, Node* p4); //Merge two portions of a DoublyLinkedList, one from the node pointed by p1 to the node pointed by p2 and
															  //the other from the node pointed by p3 to the node pointed by p4.
	void InsertionSort(Node* p1, Node* p2); //Implement InsertionSort.  Sort a portion of a DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.

	//The function Merge can be called by functions MergeSortIte and MergeSortRec;

};

/*
In-place sorting. Values have not been changed. Only nodes have been moved around.
*/
void DoublyLinkedList::MergeSortIte(Node* p1, Node* p2) {
	//If 0 or 1 nodes
	if (!p1 || !p2 || p1 == p2) return;

	Node* temp1 = p1->previous;
	Node* temp2 = p2->next;
	if (temp1) temp1->next = nullptr;
	if (temp2) temp2->previous = nullptr;
	p1->previous = nullptr;
	p2->next = nullptr;

	//Count total number of nodes
	int count = 1;
	while (p1 != p2) {
		++count;
		if (p2->previous) p2 = p2->previous;
	}

	/*
	start1 - points to the first node of first block to merge - passed to merge as p1
	start2 - points to the first node of second block to merge - passed to merge as p3
	end1 - points to the last node of first block to merge - passed to merge as p2
	end2 - points to the last node of second block to merge - passed to merge as p4
	endLast - points to the last node of the previous merged block
	*/
	Node* start1 = p1;
	Node* start2 = nullptr;
	Node* end1 = nullptr;
	Node* end2 = nullptr;
	Node* endLast = nullptr;

	//Gap size increases from 1, 2, 4... to total number of nodes
	for (int size = 1; size < count; size *= 2) {
		//start iterating from the head
		start1 = p1;
		while (start1) {

			int flag = (start1 == p1) ? 1 : 0;

			//Set end1 to end of first list
			end1 = start1;
			int end = size;
			while (--end && end1->next) end1 = end1->next;

			//Set start2 to start of second list
			start2 = end1->next;
			if (!start2) break;

			//Split the two lists
			start2->previous = nullptr;
			end1->next = nullptr;

			//Set end2 to end of second list
			end2 = start2;
			end = size;
			while (--end && end2->next) end2 = end2->next;

			//Save start of next 2 lists
			Node* nextStart = end2->next;

			//Seperate second list
			if(nextStart) nextStart->previous = nullptr;
			end2->next = nullptr;

			//Merge the two lists as one
			Merge(start1, end1, start2, end2);

			//Track the start and end back after merging
			while (start1->previous) start1 = start1->previous;
			while (end2->next) end2 = end2->next;

			if (flag) p1 = start1;
			//Add list to appropriate place
			else {
				endLast->next = start1;
				start1->previous = endLast;
			}

			endLast = end2;
			start1 = nextStart;
		}

		if(endLast) endLast->next = start1;
		if(start1) start1->previous = endLast;
	}

	while (p1->previous) p1 = p1->previous;
	p2 = p1;
	while (p2->next) p2 = p2->next;

	if (temp1) {
		temp1->next = p1;
		p1->previous = temp1;
	}
	if (temp2) {
		p2->next = temp2;
		temp2->previous = p2;
	}

}

/*
In-place sorting. Values have not been changed. Only nodes have been moved around.
*/
void DoublyLinkedList::MergeSortRec(Node* p1, Node* p2) {
	//0 or 1 nodes
	if (!p1 || !p2 || p1 == p2) return;

	Node* temp1 = p1->previous;
	Node* temp2 = p2->next;
	if (temp1) temp1->next = nullptr;
	if (temp2) temp2->previous = nullptr;
	p1->previous = nullptr;
	p2->next = nullptr;

	Node* slow = p1;
	Node* fast = p1;
	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}
	slow = slow->previous;
	Node* p3 = slow;
	Node* p4 = slow->next;
	//p3 points to end of first list
	//p4 points to start of second list
	//split the list
	p3->next = nullptr;
	p4->previous = nullptr;
	
	MergeSortRec(p1, p3);
	MergeSortRec(p4, p2);

	//Track back pointers to proper values
	while (p1->previous) p1 = p1->previous;
	while (p4->previous) p4 = p4->previous;
	while (p3->next) p3 = p3->next;
	while (p2->next) p2 = p2->next;

	Merge(p1, p3, p4, p2);

	while (p1->previous) p1 = p1->previous;
	while (p2->next) p2 = p2->next;

	if (temp1) {
		temp1->next = p1;
		p1->previous = temp1;
	}
	if (temp2) {
		p2->next = temp2;
		temp2->previous = p2;
	}

}

void DoublyLinkedList::Merge(Node* p1, Node* p2, Node* p3, Node* p4) {

	if (!p1 || !p2 || !p3 || !p4) return;

	while (p1 && p3) {
		//Do nothing, just move p1
		if (p1->value <= p3->value) {
			p1 = p1->next;
		}
		else {
			//add p3 before p1
			Node* temp = p3->next;

			//isolate p3
			if (p3->previous) p3->previous->next = p3->next;
			if (p3->next) p3->next->previous = p3->previous;

			if (p1->previous) p1->previous->next = p3;
			p3->previous = p1->previous;
			p3->next = p1;
			p1->previous = p3;

			//set head if changed
			if (p1 == head) head = p3;
			//set tail if changed
			if (p3 == tail) tail = p2;

			p3 = temp;
		}
	}
	if (p3) {
		if(p2) p2->next = p3;
		p3->previous = p2;
	}
}

void DoublyLinkedList::InsertionSort(Node* p1, Node* p2) {
	//0 or 1 nodes
	if (!p1 || p1 == p2) return;

	Node* temp1 = p1->previous;
	Node* temp2 = p2->next;
	if (temp1) temp1->next = nullptr;
	if (temp2) temp2->previous = nullptr;
	p1->previous = nullptr;
	p2->next = nullptr;

	int count = 0;
	while (p1 != p2) {
		++count;
		p2 = p2->previous;
	}
	p2 = tail;
	Node* n = p1->next;
	for (int i = 0; i < count; ++i) {
		Node* c = n->next;
		while (n->previous && n->previous->value > n->value) {

			//swap temp and temp->prev
			Node* p = n->previous;
			if (p->previous) p->previous->next = n;
			else p1 = n;
			if (p == head) head = n;
			if (n->next) n->next->previous = p;
			else p2 = p;
			if (n == tail) tail = p;

			n->previous = p->previous;
			p->next = n->next;

			n->next = p;
			p->previous = n;
		}
		n = c;
	}

	while (p1->previous) p1 = p1->previous;
	while (p2->next) p2 = p2->next;

	if (temp1) {
		temp1->next = p1;
		p1->previous = temp1;
	}
	if (temp2) {
		p2->next = temp2;
		temp2->previous = p2;
	}
}

void DoublyLinkedList::makeRandomList(int m, int n) {

	for (int i = 0; i < m; i++) {
		Node* p1 = new Node(rand() % n);
		p1->previous = tail;
		if (tail) tail->next = p1;
		tail = p1;
		if (!head) head = p1;
	}
}

void DoublyLinkedList::printForward() {
	cout << endl;
	Node* p1 = head;
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->next;
	}
}

void DoublyLinkedList::printBackward() {
	cout << endl;
	Node* p1 = tail;
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->previous;
	}
}

int main() {
	int a{ 50 }, b{ 30 };
	DoublyLinkedList d1, d2, d3;
	d1.makeRandomList(a, b);
	d1.printForward();
	d1.printBackward();

	d1.MergeSortIte(d1.head, d1.tail);
	d1.printForward();
	d1.printBackward();


	d2.makeRandomList(a, b);
	d2.printForward();
	d2.printBackward();

	d2.MergeSortRec(d2.head, d2.tail);
	d2.printForward();
	d2.printBackward();


	d3.makeRandomList(a, b);
	d3.printForward();
	d3.printBackward();

	d3.InsertionSort(d3.head, d3.tail);
	d3.printForward();
	d3.printBackward();

	return 0;
}