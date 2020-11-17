//CSE674 HW7  Due: 11:59PM, Nov. 16 (Monday)
#include <iostream>
using namespace std;

class node {
public:
	node* p_parent;
	node* p_Lchild;
	node* p_Rchild;
	int value;
	bool color; //true: red, false: black
	node() { p_parent = p_Lchild = p_Rchild = nullptr; }
	node(int i) { value = i; p_parent = p_Lchild = p_Rchild = nullptr; }
};

class tree {
public:
	node* root;
	tree() { root = nullptr; }
	void R_rotate(node* p);//R-rotate at node pointed by p
	void L_rotate(node* p);//L-rotate at node pointed by p
	//R_rotate and L_rotate will be called by insert_node, delete_node, delete_1, delete_2A, delete_2B, and delete_3.


	void insert_node(int i);
	//For insert, the new node should be the first of nodes with the same value in the order of 
	//inorder traversal.


	void delete_node(int i);
	//For delete, the deleted node shoud be the last of nodes with the same value in the order of
	//inorder traversal.
	//For replacement, always recursively replace it with predecessor, unless there is no predecessor. (In this
	//case, replace it with sucessor.)


	void delete_1(node* p); //case 1 of delete_node; p points to the parent of double black node
	void delete_2A(node* p); //case 2A of delete_node; p points to the parent of double black node
	void delete_2B(node* p);//case 2B of delete_node; p points to the parent of double black node
	void delete_3(node* p);//case 3 of delete_node; p points to the parent of double black node
	//All other cases of delete_node should be directly implemented inside the delete_node.

	//For the following, during traveral, print (value, color) of each node.
	//For color, print 1 for red and 0 for black.
	void InorderPrint(node* p);//Print all nodes of a subtree rooted at node pointed by p
	void PostorderPrint(node* p);
	void PreorderPrint(node* p);
};


int main() {



	return 0;
}