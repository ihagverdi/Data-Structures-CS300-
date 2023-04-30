/*
29. April. 2023
Program owner: Hagverdi Ibrahimli
Program content: Implementing Binary search tree and AVL tree to simulate a phonebook program.
Functions allowed: "Add new contact", "Delete contact", "Search contact", "Print Trees" with certain modifications.
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <stack>
using namespace std;

//Contact struct to represent a contact
struct contact {
	string firstName;
	string lastName;
	string phoneNumber;
	string city;
	contact(string _firstName, string _lastName, string _phoneNum, string _city)
		: firstName(_firstName), lastName(_lastName), phoneNumber(_phoneNum), city(_city)
	{
	}
};

/*Utiltiy functions to help with the BST and AVL class operations*/
void printContacts(vector<contact>& contacts) {
	for (contact& c : contacts)
		cout << c.firstName << " " << c.lastName << " " << c.phoneNumber << " " << c.city << endl;


}
bool isFirstAlphabetically(const string& string1, const string& string2) {
	for (int i = 0; i < string1.length() && i < string2.length(); i++) {
		if (string1[i] < string2[i]) {
			return true; // string1 comes first
		}
		else if (string1[i] > string2[i]) {
			return false; // string2 comes first
		}
	}
	return (string1.length() < string2.length()); // string1 comes first if it is shorter
}

const string toUpperCase(string& my_string) {
	for (char& c : my_string) {
		c = toupper(c);
	}
	return my_string;
}

bool checkTreeBalance(const int& heightLeft, const int& heightRight) {
	if ((heightLeft - heightRight) >= 2 || (heightLeft - heightRight) <= -2)
		return false;
	return true;
}
/*End of the Utility functions*/


/*Template class implementation of the Binary search tree*/
template <class C> 
class BinarySearchTree {
public:
	explicit BinarySearchTree() :
		root(NULL) {}

	~BinarySearchTree() {
		makeEmpty(root);
	}
	const contact& findMin() const {
		return findMin(root)->contactInfo;
	}
	const contact& findMax() const {
		return findMax(root)->contactInfo;
	}
	const vector<contact> find(const string& firstName, const string& lastName) const {
		vector<contact> matches;
		if (lastName.empty()) {
			searchByFirstName(root, firstName, matches);
		}
		else {
			searchByFullName(root, firstName, lastName, matches);
		}
		return matches;
	}
	bool isEmpty() const {
		return (root == NULL);
	}
	void InOrderPrintToFile() const {
		ofstream outFile("phonebookInOrderBST.txt");
		if (!outFile.is_open())
			cerr << "Could not create and open the file." << endl;
		else
			InOrderPrintToFile(root, outFile);
		outFile.close();
	}

	void PreOrderPrintToFile() const {
		ofstream outFile("phonebookPreOrderBST.txt");
		if (!outFile.is_open())
			cerr << "Could not create and open the file." << endl;
		else
			PreOrderPrintToFile(root, outFile);
		outFile.close();
	}
	void makeEmpty() {
		makeEmpty(root);
	}
	bool insert(const contact& newContact) {
		bool inserted = false;
		insert(newContact, root, inserted);
		return inserted;
	}
	bool remove(const string& firstName, const string& lastName) {
		bool removed = false;
		remove(firstName, lastName, root, removed);
		return removed;
	}
	int height() const {
		return height(root);
	}
	int heightLeftSubTree() const {
		return height(root->left);
	}
	int heightRightSubTree() const {
		return height(root->right);
	}

	void printTree() const {
		ofstream outFile("phonebookTreeBST.txt");
		if (!outFile.is_open())
			cerr << "Could not create and open the file." << endl;
		else
			printTree(root, "", false, outFile);
		outFile.close();
	}
private:
	struct Node {
		C contactInfo;
		Node* left;
		Node* right;
		Node(const contact& contact,
			Node* lt, Node* rt)
			: contactInfo(contact), left(lt), right(rt) { }
	};
	Node* root;
	void insert(const contact& newContact, Node*& rt, bool & inserted) {
		if (rt == NULL) {  // create a new node at the right place
			rt = new Node(newContact, NULL, NULL);
			inserted = true;
		}
		else if (newContact.firstName == rt->contactInfo.firstName && newContact.lastName == rt->contactInfo.lastName) {
			//is a duplicate -> don't allow
			inserted = false;
		}
		else if (isFirstAlphabetically((rt->contactInfo.firstName + rt->contactInfo.lastName), (newContact.firstName + newContact.lastName)))
			insert(newContact, rt->right, inserted);
		else
			insert(newContact, rt->left, inserted);
	}
	void remove(const string& firstName, const string& lastName, Node*& rt, bool& removed) {
		if (rt == NULL)
			return;	
		else if (isFirstAlphabetically((rt->contactInfo.firstName + rt->contactInfo.lastName), (firstName + lastName)))
			remove(firstName, lastName, rt->right, removed);
		else if (firstName == rt->contactInfo.firstName && lastName == rt->contactInfo.lastName) {
			if (rt->left != NULL && rt->right != NULL) {
				//it is found and it has two children
				rt->contactInfo = findMin(rt->right)->contactInfo;
				remove(rt->contactInfo.firstName, rt->contactInfo.lastName, rt->right, removed);
			}
			else {
				//it has either one child or no children
				Node* temp = rt;
				rt = (rt->left == NULL) ? rt->right : rt->left;
				delete temp;
			}
			removed = true;
		}
		else {
			remove(firstName, lastName, rt->left, removed);
		}
	}
	void printTree(Node* const& curr, string indent, bool last, ofstream& outFile) const {
		if (curr == NULL) {
			return;
		}
		outFile << indent;
		if (last) {
			outFile << "|__";
			indent += "   ";
		}
		else {
			outFile << "|--";
			indent += "|  ";
		}
		outFile << curr->contactInfo.firstName << " " << curr->contactInfo.lastName << endl;
		if (curr->left != NULL) {
			printTree(curr->left, indent, curr->right == NULL, outFile);
		}
		if (curr->right != NULL) {
			printTree(curr->right, indent, true, outFile);
		}
	}

	Node* findMin(Node* rt) const {
		while (rt != NULL && rt->left != NULL) {
			rt = rt->left;
		}
		return rt;
	}
	Node* findMax(Node* rt) const {
		while (rt != NULL && rt->right != NULL) {
			rt = rt->right;
		}
		return rt;
	}
	void searchByFirstName(Node* const& rt, const string& firstName, vector<contact>& matches) const {
		if (rt == NULL)
			return;
		//Inorder search
		searchByFirstName(rt->left, firstName, matches);
		if (rt->contactInfo.firstName.find(firstName) == 0) {
			matches.push_back(rt->contactInfo);
		}
		searchByFirstName(rt->right, firstName, matches);
	}
	void searchByFullName(Node* const& rt, const string& firstName, const string& lastName, vector<contact>& matches) const {
		if (rt == NULL)
			return;
		//inorder search
		searchByFullName(rt->left, firstName, lastName, matches);
		if (rt->contactInfo.firstName == firstName && rt->contactInfo.lastName.find(lastName) == 0) {
			matches.push_back(rt->contactInfo);
		}
		searchByFullName(rt->right, firstName, lastName, matches);
	}
	void makeEmpty(Node*& rt) {
		if (rt == NULL)
			return;
		makeEmpty(rt->left);
		makeEmpty(rt->right);
		delete rt;
		rt = NULL;
	}
	void InOrderPrintToFile(Node* const& rt, ofstream& outFile) const {
		if (rt == NULL) {
			return;
		}
		InOrderPrintToFile(rt->left, outFile);
		outFile << rt->contactInfo.firstName << " " << rt->contactInfo.lastName << " " << rt->contactInfo.phoneNumber << " " << rt->contactInfo.city << endl;
		InOrderPrintToFile(rt->right, outFile);
	}
	void PreOrderPrintToFile(Node* const& rt, ofstream& outFile) const {
		if (rt == NULL)
			return;
		outFile << rt->contactInfo.firstName << " " << rt->contactInfo.lastName << " " << rt->contactInfo.phoneNumber << " " << rt->contactInfo.city << endl;
		PreOrderPrintToFile(rt->left, outFile);
		PreOrderPrintToFile(rt->right, outFile);
	}
	int height(Node*& rt) const {
		if (rt == NULL)
			return -1;
		return max(height(rt->left), height(rt->right)) + 1;
	}
};
/*End of the Binary search tree class implementation*/

/*Template class implementation of the AVL tree*/
template <class C>
class AVLtree
{
public:
	explicit AVLtree() : root(NULL) {}
	~AVLtree() {
		makeEmpty(root);
	}
	const contact& findMin() const {
		return findMin(root)->contactInfo;
	}
	const contact& findMax() const {
		return findMax(root)->contactInfo;
	}
	const vector<contact> find(const string& firstName, const string& lastName) const {
		vector<contact> matches;
		if (lastName.empty()) {
			searchByFirstName(root, firstName, matches);
		}
		else {
			searchByFullName(root, firstName, lastName, matches);
		}
		return matches;
	}
	bool isEmpty() const {
		return (root == NULL);
	}
	void InOrderPrintToFile() const {
		ofstream outFile("phonebookInOrderAVL.txt");
		if (!outFile.is_open())
			cerr << "Could not create and open the file." << endl;
		else
			InOrderPrintToFile(root, outFile);
		outFile.close();
	}
	void PreOrderPrintToFile() const {
		ofstream outFile("phonebookPreOrderAVL.txt");
		if (!outFile.is_open())
			cerr << "Could not create and open the file." << endl;
		else
			PreOrderPrintToFile(root, outFile);
		outFile.close();
	}
	void makeEmpty() {
		makeEmpty(root);
	}
	bool insert(const contact& newContact) {
		bool inserted = false;
		insert(newContact, root, inserted);
		return inserted;
	}
	void remove(const string& firstName, const string& lastName) {
		remove(firstName, lastName, root);
	}
	int heightLeftSubTree() {
		return height(root->left);
	}
	int heightRightSubTree() {
		return height(root->right);
	}
	void printTree() const {
		ofstream outFile("phonebookTreeAVL.txt");
		if (!outFile.is_open())
			cerr << "Could not create and open the file." << endl;
		else
			printTree(root, "", false, outFile);
		outFile.close();
	}
private:
	struct Node
	{
		C contactInfo;
		Node* left;
		Node* right;
		int height;
		Node(const contact& contact,
			Node* lt, Node* rt)
			: contactInfo(contact), left(lt), right(rt), height(-1) { }
	};
	Node* root;
	void insert(const contact& newContact, Node*& rt, bool& inserted) {
		if (rt == NULL) {
			rt = new Node(newContact, NULL, NULL);
			inserted = true;
		}
		else if (isFirstAlphabetically((rt->contactInfo.firstName + rt->contactInfo.lastName), (newContact.firstName + newContact.lastName))) {
			insert(newContact, rt->right, inserted);
			if (height(rt->right) - height(rt->left) == 2) {
				// height of the right subtree increased
				if (isFirstAlphabetically((rt->right->contactInfo.firstName + rt->right->contactInfo.lastName), (newContact.firstName + newContact.lastName)))
					// X was inserted to right-right subtree
					rotateWithRightChild(rt);
				else // X was inserted to right-left subtree
					doubleWithRightChild(rt);
			}
		}
		else {
			insert(newContact, rt->left, inserted);
			// Check if the left tree is out of balance (left subtree grew in height!)
			if (height(rt->left) - height(rt->right) == 2) {
				if (isFirstAlphabetically((rt->left->contactInfo.firstName + rt->left->contactInfo.lastName), (newContact.firstName + newContact.lastName)))
					doubleWithLeftChild(rt);
				else // X was inserted to the left-left subtree!
					rotateWithLeftChild(rt);
			}
		}
		rt->height = max(height(rt->left), height(rt->right)) + 1;
	}
	void remove(const string& firstName, const string& lastName, Node*& rt) {
		if (rt == NULL)
			return;
		else if (isFirstAlphabetically((rt->contactInfo.firstName + rt->contactInfo.lastName), (firstName + lastName)))
			remove(firstName, lastName, rt->right);
		else if (firstName == rt->contactInfo.firstName && lastName == rt->contactInfo.lastName) {
			if (rt->left != NULL && rt->right != NULL) {
				//it is found and it has two children
				rt->contactInfo = findMin(rt->right)->contactInfo;
				remove(rt->contactInfo.firstName, rt->contactInfo.lastName, rt->right);
			}
			else {
				//it has either one child or no children
				Node* temp = rt;
				rt = (rt->left == NULL) ? rt->right : rt->left;
				delete temp;
			}
		}
		else {
			remove(firstName, lastName, rt->left);
			// Check if the left tree is out of balance (left subtree grew in height!)

		}
		if (rt == NULL)
			return; //deleted node has no children
		rt->height = max(height(rt->left), height(rt->right)) + 1;

		int balance = getBalance(rt);

		// If this node becomes unbalanced,
		// then there are 4 cases

		// Left Left Case
		if (balance > 1 &&
			getBalance(rt->left) >= 0)
			rotateWithLeftChild(rt);

		// Left Right Case
		else if (balance > 1 &&
			getBalance(rt->left) < 0)
			doubleWithLeftChild(rt);

		// Right Right Case
		else if (balance < -1 &&
			getBalance(rt->right) <= 0)
			rotateWithRightChild(rt);

		// Right Left Case
		else if (balance < -1 &&
			getBalance(rt->right) > 0)
			doubleWithRightChild(rt);
	}
	Node* findMin(Node* rt) const {
		while (rt != NULL && rt->left != NULL) {
			rt = rt->left;
		}
		return rt;
	}
	Node* findMax(Node* rt) const {
		while (rt != NULL && rt->right != NULL) {
			rt = rt->right;
		}
		return rt;
	}
	void searchByFirstName(Node* const& rt, const string& firstName, vector<contact>& matches) const {
		if (rt == NULL)
			return;
		//Inorder search
		searchByFirstName(rt->left, firstName, matches);
		if ((rt->contactInfo.firstName).find((firstName)) == 0) {
			matches.push_back(rt->contactInfo);
		}
		searchByFirstName(rt->right, firstName, matches);
	}
	void searchByFullName(Node* const& rt, const string& firstName, const string& lastName, vector<contact>& matches) const {
		if (rt == NULL)
			return;
		//inorder search
		searchByFullName(rt->left, firstName, lastName, matches);
		if (((rt->contactInfo.firstName) == (firstName)) && ((rt->contactInfo.lastName).find((lastName)) == 0)) {
			matches.push_back(rt->contactInfo);
		}
		searchByFullName(rt->right, firstName, lastName, matches);
	}
	void makeEmpty(Node*& rt) {
		if (rt == NULL)
			return;
		makeEmpty(rt->left);
		makeEmpty(rt->right);
		delete rt;
		rt = NULL;
	}
	void InOrderPrintToFile(Node* rt, ofstream& outFile) const {
		if (rt == NULL) {
			return;
		}
		InOrderPrintToFile(rt->left, outFile);
		outFile << rt->contactInfo.firstName << " " << rt->contactInfo.lastName << " " << rt->contactInfo.phoneNumber << " " << rt->contactInfo.city << endl;
		InOrderPrintToFile(rt->right, outFile);
	}
	void PreOrderPrintToFile(Node* rt, ofstream& outFile) const {
		if (rt == NULL) {
			return;
		}
		outFile << rt->contactInfo.firstName << " " << rt->contactInfo.lastName << " " << rt->contactInfo.phoneNumber << " " << rt->contactInfo.city << endl;
		PreOrderPrintToFile(rt->left, outFile);
		PreOrderPrintToFile(rt->right, outFile);
	}
	//AVL manipulations
	int height(Node* rt) const {
		return (rt == NULL ? -1 : rt->height);
	}
	int max(int lhs, int rhs) const {
		return (lhs > rhs ? lhs : rhs);
	}
	void rotateWithLeftChild(Node*& k2) const {
		Node* k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(height(k2->left), height(k2->right)) + 1;
		k1->height = max(height(k1->left), k2->height) + 1;
		k2 = k1;
	}
	void rotateWithRightChild(Node*& k1) const {
		Node* k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(height(k1->left), height(k1->right)) + 1;
		k2->height = max(height(k2->right), k1->height) + 1;
		k1 = k2;
	}
	void doubleWithLeftChild(Node*& k3) const {
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}
	void doubleWithRightChild(Node*& k1) const {
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}
	int getBalance(Node* node) {
		return (height(node->left) - height(node->right));
	}
	void printTree(Node* const& curr, string indent, bool last, ofstream& outFile) const {
		if (curr == NULL)
			return;

		outFile << indent;
		if (last) {
			outFile << "|__";
			indent += "   ";
		}
		else {
			outFile << "|--";
			indent += "|  ";
		}

		outFile << curr->contactInfo.firstName << " " << curr->contactInfo.lastName << endl;

		printTree(curr->left, indent, curr->right == NULL, outFile);
		printTree(curr->right, indent, true, outFile);
	}
};
/*End of the AVL tree class implementation*/

/*Wrap-up performance measuring functions*/
long long searchInBST(BinarySearchTree<contact>& BST, const string & firstName, const string & lastName) {
	auto start_time = chrono::high_resolution_clock::now();
	vector<contact> result = BST.find(firstName, lastName);
	auto end_time = chrono::high_resolution_clock::now();
	printContacts(result);
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long searchInAVL(AVLtree<contact>& AVL, const string& firstName, const string& lastName) {
	auto start_time = chrono::high_resolution_clock::now();
	vector<contact> result = AVL.find(firstName, lastName);
	auto end_time = chrono::high_resolution_clock::now();
	printContacts(result);
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long deleteFromBST(BinarySearchTree<contact>& BST, bool& success, const string& firstName, const string& lastName) {
	auto start_time = chrono::high_resolution_clock::now();
	success = BST.remove(firstName, lastName);
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long deleteFromAVL(AVLtree<contact>& AVL, const string& firstName, const string& lastName) {
	auto start_time = chrono::high_resolution_clock::now();
	AVL.remove(firstName, lastName);
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long addToBST(BinarySearchTree<contact>& BST, const contact& newContact, bool& added) {
	auto start_time = chrono::high_resolution_clock::now();
	added = BST.insert(newContact);
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long addToAVL(AVLtree<contact>& AVL, const contact& newContact, bool& added) {
	auto start_time = chrono::high_resolution_clock::now();
	added = AVL.insert(newContact);
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long InOrderPrintToFile(BinarySearchTree<contact> & BST) {
	auto start_time = chrono::high_resolution_clock::now();
	BST.InOrderPrintToFile();
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long InOrderPrintToFile(AVLtree<contact>& AVL) {
	auto start_time = chrono::high_resolution_clock::now();
	AVL.InOrderPrintToFile();
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long PreOrderPrintToFile(BinarySearchTree<contact>& BST) {
	auto start_time = chrono::high_resolution_clock::now();
	BST.PreOrderPrintToFile();
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long PreOrderPrintToFile(AVLtree<contact>& AVL) {
	auto start_time = chrono::high_resolution_clock::now();
	AVL.PreOrderPrintToFile();
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long printTreeToFile(BinarySearchTree<contact>& BST) {
	auto start_time = chrono::high_resolution_clock::now();
	BST.printTree();
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long printTreeToFile(AVLtree<contact>& AVL) {
	auto start_time = chrono::high_resolution_clock::now();
	AVL.printTree();
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
}

long long makeBST(BinarySearchTree<contact>& BST, ifstream& input_file) {
	auto start_time = chrono::high_resolution_clock::now();
	string fname, lname, city, phoneNum;
	while (input_file) {
		input_file >> fname >> lname >> phoneNum >> city;
		if (input_file) {
			contact newContact(toUpperCase(fname), toUpperCase(lname), phoneNum, toUpperCase(city));
			BST.insert(newContact);
		}
	}
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
}

long long makeAVL(AVLtree<contact>& AVL, ifstream& input_file) {
	auto start_time = chrono::high_resolution_clock::now();
	string fname, lname, city, phoneNum;
	while (input_file) {
		input_file >> fname >> lname >> phoneNum >> city;
		if (input_file) {
			contact newContact(toUpperCase(fname), toUpperCase(lname), phoneNum, toUpperCase(city));
			AVL.insert(newContact);
		}
	}
	auto end_time = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
}
/*End of wrap-up performance measuring functions*/

//Function to handle all operations systematically depending on the input of user
void prompt(BinarySearchTree<contact>& BST, AVLtree<contact>& AVL) {
	int choice;
	do {
		cout << "Choose which action to perform from 1 to 6: " << endl;
		cout << "1 - Search a phonebook contact" << endl;
		cout << "2 - Adding a phonebook contact" << endl;
		cout << "3 - Deleting a phonebook contact" << endl;
		cout << "4 - Print the phonebook to a file (inorder)" << endl << "    " << "Print the phoenbook to a file(preorder)" << endl;
		cout << "5 - Draw the phonebook as a Tree to a file" << endl;
		cout << "6 - Press 6 to exit" << endl;
		cin >> choice;
		cout << endl;
		switch (choice) {
		case 1: {
			string input, firstName, lastName;
			cout << "Search for a contact: ";
			cin.ignore();
			getline(cin, input);
			stringstream ss(input);
			ss >> firstName >> lastName;
			firstName = toUpperCase(firstName);
			lastName = toUpperCase(lastName);
			//first BST
			cout << endl;
			cout << "Searching an item in the phonebook  (BST) . . ." << endl << endl;
			cout << "Phonebook: Searching for: (" << firstName << " " << lastName << ")" << endl;
			cout << "====================================" << endl;
			long long timeBST = searchInBST(BST, firstName, lastName);
			cout << endl << endl;
			//second AVL
			cout << "Searching an item in the phonebook  (AVL) . . ." << endl << endl;
			cout << "Phonebook: Searching for: (" << firstName << " " << lastName << ")" << endl;
			cout << "====================================" << endl;
			long long timeAVL = searchInAVL(AVL, firstName, lastName);
			cout << endl << endl;
			cout << "The search in BST took " << timeBST << " nanonseconds..." << endl;
			cout << "The search in AVL took " << timeAVL << " nanonseconds..." << endl;
			cout << endl << endl;
			break;
		}
		case 2: {
			string fullName, firstName, lastName, phoneNumber, city;
			cout << "Enter the information of the contact to be added: " << endl;
			cout << "Name: ";
			cin.ignore();
			getline(cin, fullName);
			stringstream ss(fullName);
			ss >> firstName >> lastName;
			cout << endl;
			cout << "Tel: ";
			cin >> phoneNumber;
			cout << endl;
			cout << "City: ";
			cin >> city;
			cout << endl;
			firstName = toUpperCase(firstName);
			lastName = toUpperCase(lastName);
			city = toUpperCase(city);
			contact newContact(firstName, lastName, phoneNumber, city);
			//Add to BST 
			bool success;	
			cout << "Adding an item to the phonebook (BST) . . ." << endl;
			cout << "====================================" << endl;
			long long timeBST = addToBST(BST, newContact, success);
			if (success)
				cout << "Contact has been added successfully to the BST" << endl;
			else
				cout << "Contact not added. Duplicate exists." << endl;
			cout << endl << endl;
			//add to AVL
			cout << "Adding an item to the phonebook AVL . . ." << endl;
			cout << "====================================" << endl;
			long long timeAVL = addToAVL(AVL, newContact, success);
			if (success) {
				cout << "Contact has been added successfully to the AVL" << endl;
				cout << endl << endl;
				cout << "Adding a contact to the Binary Tree took " << timeBST << " nanoseconds. . ." << endl;
				cout << "Adding a contact to the AVL Tree took " << timeAVL << " nanoseconds. . ." << endl;
			}
			else
			cout << "Contact not added. Duplicate exists." << endl;
			cout << endl << endl;
			break;
		}
		case 3: {
			string fullName, firstName, lastName;
			cout << "Deleting an item from the phonebook . . ." << endl;
			cout << "Enter the fullname of the contact to be deleted: ";
			cin.ignore();
			getline(cin, fullName);
			cout << endl << endl;
			stringstream ss(fullName);
			ss >> firstName >> lastName;
			firstName = toUpperCase(firstName);
			lastName = toUpperCase(lastName);
			bool success;
			//remove from BST
			long long timeBST = deleteFromBST(BST, success, firstName, lastName);
			//remove from AVL
			long long timeAVL = deleteFromAVL(AVL, firstName, lastName);
			if (success) {
				cout << "Deleted succcessfully. . ." << endl << endl;
				cout << "Deletion from the Binary Tree took " << timeBST << " nanoseconds. . ." << endl;
				cout << "Deletion from the AVL Tree took " << timeAVL << " nanoseconds. . ." << endl;
			}
			else
				cout << "Deletion failed. Contact not found." << endl;
			cout << endl << endl;
			break;
		}
		case 4: {
			long long timeInOrderBST = InOrderPrintToFile(BST);
			long long timeInOrderAVL = InOrderPrintToFile(AVL);
			long long timePreOrderBST = PreOrderPrintToFile(BST);
			long long timePreOrderAVL = PreOrderPrintToFile(AVL);
			cout << "Printing in order to file from the Binary Tree took " << timeInOrderBST <<" nanoseconds." << endl;
			cout << "Printing in order to file from the AVL Tree took " << timeInOrderAVL << " nanoseconds." << endl;
			cout << "Printing pre order to file from the Binary Tree took " << timePreOrderBST << " nanoseconds." << endl;
			cout << "Printing pre order to file from the AVL Tree took " << timePreOrderAVL << " nanoseconds." << endl;
			cout << endl << endl;
			break;
		}
		case 5: {
			long long timeBST = printTreeToFile(BST);
			long long timeAVL = printTreeToFile(AVL);
			cout << "Drawing tree to file from the Binary Tree took " << timeBST << " nanoseconds. . ." << endl;
			cout << "Drawing tree to file from the AVL Tree took " << timeAVL << " nanoseconds. . ." << endl;
			cout << endl << endl;
			break;
		}
		case 6: {
			cout << "Exiting the program..." << endl << endl;
			break;
		}
		}

	} while (choice != 6);
}

void run() {
	string fileName;
	cout << "Please enter the contact file name: ";
	getline(cin, fileName);

	ifstream input_file(fileName);
	if (!input_file.is_open()) {
		cerr << "Error opening file. Please try again." << endl;
		return;	
	}
	cout << "Loading the phonebook into a BST." << endl;
	BinarySearchTree<contact> BST;
	long long timeBST = makeBST(BST, input_file);
	cout << "Phonebook creation in BST took " << timeBST << " milliseconds. . ." << endl;
	const int heightLeftSubTree = BST.heightLeftSubTree();
	const int heightRightSubTree = BST.heightRightSubTree();
	if (checkTreeBalance(heightLeftSubTree, heightRightSubTree))
		cout << "The BST is balanced";
	else
		cout << "The BST is not balanced";
	cout << endl;
	cout << "The heights of BST are for left: " << heightLeftSubTree << " and right: " << heightRightSubTree << endl;
	// Create AVL
	input_file.clear();
	input_file.seekg(0, ios::beg);
	cout << "Loading the phonebook into an AVL." << endl;
	AVLtree<contact> AVL;
	long long timeAVL = makeAVL(AVL, input_file);
	cout << "Phonebook creation in AVL took " << timeAVL << " milliseconds. . ." << endl;
	const int heightLeftSubTree_AVL = AVL.heightLeftSubTree();
	const int heightRightSubTree_AVL = AVL.heightRightSubTree();
	if (checkTreeBalance(heightLeftSubTree_AVL, heightRightSubTree_AVL))
		cout << "The AVL is balanced";
	else
		cout << "The AVL is not balanced";
	cout << endl;
	cout << "The heights of AVL are for left: " << heightLeftSubTree_AVL << " and right: " << heightRightSubTree_AVL << endl;
	cout << endl << endl;
	prompt(BST, AVL);
}

int main() {
	run();
	return 0;
}