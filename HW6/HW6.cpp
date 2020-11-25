//CSE674 HW7
//Name: Neil Malu
//NetID: nemalu
//SUID: 635692900
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

void tree::InorderPrint(node* p) {
	if (!p) return;

	InorderPrint(p->p_Lchild);
	cout << "(" << p->value << ", " << p->color << ")";
	InorderPrint(p->p_Rchild);

	if (p == root) cout << endl;
}

void tree::PostorderPrint(node* p) {
	if (!p) return;

	PostorderPrint(p->p_Lchild);
	PostorderPrint(p->p_Rchild);
	cout << "(" << p->value << ", " << p->color << ")";

	if (p == root) cout << endl;
}

void tree::PreorderPrint(node* p) {
	if (!p) return;

	cout << "(" << p->value << ", " << p->color << ")";
	PreorderPrint(p->p_Lchild);
	PreorderPrint(p->p_Rchild);
	
	if (p == root) cout << endl;
}

void tree::insert_node(int i) {
	node* newNode = new node(i);
	newNode->color = 1;

	if (!root) {
		root = newNode;
		newNode->color = 0;
		return;
	}

	node* curr = root;
	while (true) {
		if (newNode->value <= curr->value) {
			if (curr->p_Lchild) curr = curr->p_Lchild;
			else break;
		}
		else {
			if (curr->p_Rchild) curr = curr->p_Rchild;
			else break;
		}
	}

	//curr points to parent of newNode
	if (newNode->value <= curr->value) curr->p_Lchild = newNode;
	else curr->p_Rchild = newNode;
	newNode->p_parent = curr;

	//Fix violations
	//Violation only occurs when parent is red
	//Since parent is red, it cannot be root, so it will always have parent
	node* parent = curr;
	while(parent->color) {
		node* uncle;
		node* gp = parent->p_parent;
		//Patterns:
		//LL: 1
		//LR: 2
		//RL: 3
		//RR: 4
		int pattern = 0;
		//if parent is left child, then uncle is right
		if (gp->p_Lchild == parent) {
			uncle = gp->p_Rchild;
			pattern = 1;
		}
		else {
			uncle = gp->p_Lchild;
			pattern = 3;
		}

		//if uncle is red
		if (uncle && uncle->color) {
			//can propogate
			gp->color = 1;
			parent->color = 0;
			uncle->color = 0;
			if (gp == root) {
				gp->color = 0;
				return;
			}
			//this will propogate
			newNode = gp;
			parent = gp->p_parent;
		}
		else {
			//check for pattern, rotate, recolor and return.
			//No propogation
			//if uncle is right child
			if (pattern == 1) {
				//if newNode is left child then LL
				//else LR
				if (parent->p_Rchild == newNode) pattern = 2;
			}
			else {
				//if newNode is left child then RL
				//else RR
				if (parent->p_Rchild == newNode) pattern = 4;
			}

			switch (pattern) {
				case 1:
					R_rotate(gp);
					gp->color = 1;
					parent->color = 0;
					return;
				case 2:
					L_rotate(parent);
					R_rotate(gp);
					newNode->color = 0;
					gp->color = 1;
					return;
				case 3:
					R_rotate(parent);
					L_rotate(gp);
					newNode->color = 0;
					gp->color = 1;
					return;
				case 4:
					L_rotate(gp);
					gp->color = 1;
					parent->color = 0;
					return;
			}
		}
	}
}

void tree::L_rotate(node* p) {
	//L_Rotate means p will always have a right child
	//L_Rotate means p's right will become p's parent and p will become its left child
	//If r_child has l_child, then that l_child will become p's r_child

	node* funny = p->p_Rchild->p_Lchild;

	node* parent = p->p_parent;

	if (parent) {
		//if p is left child
		if (parent->p_Lchild == p) parent->p_Lchild = p->p_Rchild;
		else parent->p_Rchild = p->p_Rchild;
	}
	else root = p->p_Rchild;

	p->p_Rchild->p_parent = parent;
	p->p_Rchild->p_Lchild = p;
	p->p_parent = p->p_Rchild;
	p->p_Rchild = funny;

	if (funny) funny->p_parent = p;
}

void tree::R_rotate(node* p) {
	//R_Rotate means p will always have a left child
	//R_Rotate means p's left will become p's parent and p will become its right child

	node* funny = p->p_Lchild->p_Rchild;
	node* parent = p->p_parent;

	if (parent) {
		if (parent->p_Lchild == p) parent->p_Lchild = p->p_Lchild;
		else parent->p_Rchild = p->p_Lchild;
	}
	else root = p->p_Lchild;

	p->p_Lchild->p_parent = parent;
	p->p_Lchild->p_Rchild = p;
	p->p_parent = p->p_Lchild;
	p->p_Lchild = funny;

	if (funny) funny->p_parent = p;
}

void tree::delete_node(int i) {
	if (!root) return;

	if (!root->p_Lchild && !root->p_Rchild && root->value == i) {
		delete root;
		root = nullptr;
		return;
	}

	node* curr = root;
	node* del = nullptr;
	//curr is the iterator. del is the node to be deleted

	while (true) {
		if (curr->value < i) {
			if (curr->p_Rchild) curr = curr->p_Rchild;
			else return;
		}
		else if (curr->value > i) {
			if (curr->p_Lchild) curr = curr->p_Lchild;
			else return;
		}
		else {
			//If we find equal node, find its highest successor
			//del is the first node with value equal
			del = curr;
			//Keep going to the right until node value is greater than target
			while (curr->p_Rchild && curr->p_Rchild->value >= i) {
				curr = curr->p_Rchild;
				if (curr->value > i) break;
				else del = curr;
			}

			//Keep going to the left until node is of same value
			while (curr->p_Lchild && curr->p_Lchild->value != i) {
				curr = curr->p_Lchild;
			}
			//If node of same value exists, update del
			if (curr->p_Lchild) del = curr;
			break;

		}
	}

	node* pred;
	node* parent = nullptr;
	//del points to the node to be deleted.
	//pred points to the predecessor that needs to be swapped
	//parent is the parent of the node that's deleted (predecessor)

	//Trivial condition: If del is red leaf
	if (del->color && !del->p_Lchild && !del->p_Rchild) {
		parent = del->p_parent;
		if (parent->p_Lchild == del) parent->p_Lchild = nullptr;
		else parent->p_Rchild = nullptr;
		del->p_parent = nullptr;
		delete del;
		return;
	}

	//Trivial condition: If del is black leaf without a sibling and parent is red
	if (!del->color && !del->p_Lchild && !del->p_Rchild && del->p_parent->color) {
		parent = del->p_parent;
		if (parent->p_Lchild && parent->p_Rchild) goto h;
		if (parent->p_Lchild == del) parent->p_Lchild = nullptr;
		else parent->p_Rchild = nullptr;
		del->p_parent = nullptr;
		parent->color = 0;
		delete del;
		return;
	}

	h:
	//Trivial condition: del is red and is parent of leaf
	if (del->color && !(del->p_Lchild && del->p_Rchild)) {
		if (del->p_Lchild && !del->p_Lchild->p_Lchild && !del->p_Lchild->p_Rchild) {
			parent = del->p_parent;
			del->p_Lchild->p_parent = parent;
			if (parent->p_Lchild == del) parent->p_Lchild = del->p_Lchild;
			else parent->p_Rchild = del->p_Lchild;
			del->p_Lchild->p_Rchild = del->p_Rchild;
			if (del->p_Rchild) del->p_Rchild->p_parent = del->p_Lchild;
			delete del;
			return;
		}
		if (del->p_Rchild && !del->p_Rchild->p_Lchild && !del->p_Rchild->p_Rchild) {
			parent = del->p_parent;
			del->p_Rchild->p_parent = parent;
			if (parent->p_Lchild == del) parent->p_Lchild = del->p_Rchild;
			else parent->p_Rchild = del->p_Rchild;
			delete del;
			return;
		}
	}

	//Trivial condition: del is black and parent of red leaf
	if (!del->color) {
		if (del->p_Lchild && !del->p_Lchild->p_Lchild && !del->p_Lchild->p_Rchild && del->p_Lchild->color) {
			parent = del->p_parent;
			del->p_Lchild->p_Rchild = del->p_Rchild;
			if (del->p_Rchild) del->p_Rchild->p_parent = del->p_Lchild;
			del->p_Lchild->color = 0;
			//If del is root
			if (!parent) {
				root = del->p_Lchild;
				del->p_Lchild->p_parent = nullptr;
			}
			else {
				del->p_Lchild->p_parent = parent;
				if (parent->p_Lchild == del) parent->p_Lchild = del->p_Lchild;
				else parent->p_Rchild = del->p_Lchild;
			}
			del->p_Lchild = nullptr;
			delete del;
			return;
		}

		if (del->p_Rchild && !del->p_Rchild->p_Lchild && !del->p_Rchild->p_Rchild && del->p_Rchild->color) {
			parent = del->p_parent;
			del->p_Rchild->color = 0;
			if (!parent) {
				root = del->p_Rchild;
				del->p_Rchild->p_parent = nullptr;
			}
			else {
				del->p_Rchild->p_parent = del->p_parent;
				if (parent->p_Lchild == del) parent->p_Lchild = del->p_Rchild;
				else parent->p_Rchild = del->p_Rchild;
			}
			del->p_Rchild = nullptr;
			delete del;
			return;
		}
	}

	//For predecessor, go once to left and all the way to the right
	pred = del->p_Lchild;
	while (pred && pred->p_Rchild) pred = pred->p_Rchild;
	int flag = 0;
	//If predecessor doesn't exist, take successor -> right and all the way to the left
	if (!pred && del->p_Rchild) {
		pred = del->p_Rchild;
		while (pred->p_Lchild) pred = pred->p_Lchild;
		flag = 1;
	}

	//If not flag, means we took pred
	if (pred && !flag) {
		//pred can never have a right child
		if (pred->p_Lchild) {
			if (pred == pred->p_parent->p_Lchild) pred->p_parent->p_Lchild = pred->p_Lchild;
			else pred->p_parent->p_Rchild = pred->p_Lchild;
			pred->p_Lchild->p_parent = pred->p_parent;
		}
	}
	else {
		//We took succ
		if (pred && pred->p_Rchild) {
			if (pred == pred->p_parent->p_Lchild) pred->p_parent->p_Lchild = pred->p_Rchild;
			else pred->p_parent->p_Rchild = pred->p_Rchild;
			pred->p_Rchild->p_parent = pred->p_parent;
		}
	}


	//If pred doesn't exist, del is left most black leaf
	if (!pred) pred = del;

	del->value = pred->value;
	parent = pred->p_parent;

	if (pred->color) {
		delete pred;
		return;
	}
	

	//Double black cases
	node* sibling;
	if (pred == parent->p_Lchild) {
		sibling = parent->p_Rchild;
		parent->p_Lchild = nullptr;
	}
	else {
		sibling = parent->p_Lchild;
		parent->p_Rchild = nullptr;
	}

	delete pred;

	while (true) {
		if (!parent) break;

		//red sibling
		if (sibling && sibling->color) {
			delete_3(parent);
			if (parent->p_Lchild) sibling = parent->p_Lchild;
			else sibling = parent->p_Rchild;
		}
		else {
			//black sibling
			

			if (!sibling) {
				if (parent->color) {
					parent->color = 0;
					return;
				}
				else delete_2A(parent);
				node* gp = parent->p_parent;
				if (gp && gp->p_Lchild == parent) sibling = gp->p_Rchild;
				if (gp && gp->p_Rchild == parent) sibling = gp->p_Lchild;
				parent = gp;
			}
			else {
				//In these cases, how to differentiate between sibling and double black???
				//Change sibling color to red
				sibling->color = 1;
				if ((sibling->p_Lchild && sibling->p_Lchild->color) || (sibling->p_Rchild && sibling->p_Rchild->color)) {
					delete_1(parent);
					return;
				}
				else {

					if (parent->color) {
						delete_2B(parent);
						return;
					}
					else delete_2A(parent);
					node* gp = parent->p_parent;
					if (gp && gp->p_Lchild == parent) sibling = gp->p_Rchild;
					if (gp && gp->p_Rchild == parent) sibling = gp->p_Lchild;
					parent = gp;
				}
			}
			
		}

	}
}

void tree::delete_1(node* parent) {
	node* sibling;

	//if double black is on the left, this is true
	//If on the right, its false
	bool leftDB = true;

	if (parent->p_Lchild) {
		if (parent->p_Lchild->color) {
			sibling = parent->p_Lchild;
			sibling->color = 0;
			leftDB = false;
		}
		else {
			sibling = parent->p_Rchild;
			sibling->color = 0;
		}
	}
	else {
		sibling = parent->p_Rchild;
		sibling->color = 0;
	}

	int pattern = 0;

	if (leftDB) {
		if (sibling->p_Rchild && sibling->p_Rchild->color) pattern = 4;
		else pattern = 3;
	}
	else {
		if (sibling->p_Lchild && sibling->p_Lchild->color) pattern = 1;
		else pattern = 2;
	}

	switch (pattern) {
		case 1:
			R_rotate(parent);
			sibling->p_Rchild->color = 0;
			sibling->color = parent->color;
			parent->color = 0;
			return;
		case 2:
			L_rotate(sibling);
			sibling = sibling->p_parent;
			R_rotate(parent);
			sibling->color = parent->color;
			parent->color = 0;
			return;
		case 3:
			R_rotate(sibling);
			sibling = sibling->p_parent;
			L_rotate(parent);
			sibling->color = parent->color;
			parent->color = 0;
			return;
		case 4:
			L_rotate(parent);
			sibling->p_Lchild->color = 0;
			sibling->color = parent->color;
			parent->color = 0;
			return;
	}

}

void tree::delete_2A(node* parent) {
	node* sibling;
	if (parent->p_Lchild) {
		if (parent->p_Lchild->color) {
			sibling = parent->p_Lchild;
			sibling->color = 0;
		}
		else {
			sibling = parent->p_Rchild;
			sibling->color = 0;
		}
	}
	else {
		sibling = parent->p_Rchild;
		sibling->color = 0;
	}
	sibling->color = 1;
}

void tree::delete_2B(node* parent) {
	node* sibling;
	if (parent->p_Lchild) {
		if (parent->p_Lchild->color) {
			sibling = parent->p_Lchild;
			sibling->color = 0;
		}
		else {
			sibling = parent->p_Rchild;
			sibling->color = 0;
		}
	}
	else {
		sibling = parent->p_Rchild;
		sibling->color = 0;
	}

	parent->color = 0;
	sibling->color = 1;
}

void tree::delete_3(node* parent) {
	node* sibling;
	//if double black is on the left, this is true
	//If on the right, its false
	bool leftDB = true;

	if (parent->p_Lchild) {
		if (parent->p_Lchild->color) {
			sibling = parent->p_Lchild;
			leftDB = false;
		}
		else {
			sibling = parent->p_Rchild;
		}
	}
	else sibling = parent->p_Rchild;

	//L rotate at parent
	if (leftDB) {
		L_rotate(parent);
		parent->color = 1;
		sibling->color = 0;
		return;
	}

	R_rotate(parent);
	parent->color = 1;
	sibling->color = 0;
}

int main() {

	tree t1 = tree();
	t1.insert_node(100);
	t1.InorderPrint(t1.root);
	t1.insert_node(80);
	t1.InorderPrint(t1.root);
	t1.insert_node(200);
	t1.InorderPrint(t1.root);
	t1.insert_node(90);
	t1.InorderPrint(t1.root);
	t1.insert_node(85);
	t1.InorderPrint(t1.root);
	t1.insert_node(70);
	t1.InorderPrint(t1.root);
	t1.insert_node(75);
	t1.InorderPrint(t1.root);
	t1.insert_node(82);
	t1.InorderPrint(t1.root);
	t1.insert_node(83);
	t1.InorderPrint(t1.root);
	t1.insert_node(80);
	t1.InorderPrint(t1.root);



	t1.delete_node(80);
	t1.InorderPrint(t1.root);
	t1.delete_node(70);
	t1.InorderPrint(t1.root);
	t1.delete_node(75);
	t1.InorderPrint(t1.root);
	t1.delete_node(85);
	t1.InorderPrint(t1.root);
	t1.delete_node(90);
	t1.InorderPrint(t1.root);
	t1.delete_node(100);
	t1.InorderPrint(t1.root);

	t1.insert_node(75);
	t1.InorderPrint(t1.root);
	t1.insert_node(81);
	t1.InorderPrint(t1.root);
	t1.delete_node(75);
	t1.InorderPrint(t1.root);
	t1.delete_node(100);
	t1.InorderPrint(t1.root);

	t1.delete_node(81);
	t1.InorderPrint(t1.root);
	t1.delete_node(200);
	t1.InorderPrint(t1.root);
	t1.delete_node(80);
	t1.InorderPrint(t1.root);
	t1.delete_node(82);
	t1.InorderPrint(t1.root);
	t1.delete_node(83);
	t1.InorderPrint(t1.root);

	t1.insert_node(75);
	t1.InorderPrint(t1.root);
	t1.insert_node(80);
	t1.InorderPrint(t1.root);
	return 0;
}