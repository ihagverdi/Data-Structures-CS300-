/*
project: HashTable Project
author: Hagverdi Ibrahimli
date: 16 May 2023
description: This program reads a text file containing contact information and stores them in a hash table and in a Binary search tree. The program also measures the performance of the hash table and a binary search tree on various operations such as find, insert, and remove.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

#define INITIAL_TABLE_SIZE 53 //initial size of the hash table
#define K 500 //Number of iterations to measure performace of BST and HashTable on find operation
#define lambda 0.7 //load factor of the hash table
struct Contact {
	string firstName;
	string lastName;
	string phoneNumber;
	string city;
	Contact() {}; //default constructor
	Contact(string _firstName, string _lastName, string _phoneNum, string _city)
		: firstName(_firstName), lastName(_lastName), phoneNumber(_phoneNum), city(_city) {};

};

void printContact(const Contact& contact) {
	cout << contact.firstName << " " << contact.lastName << " " << contact.phoneNumber << " " << contact.city << endl;
}

//States of the hash table entries
enum HashEntryStates {
	EMPTY,
	DELETED,
	ACTIVE,
};

//HashTable class implemented adopting the 
class HashTable {
private:
	struct HashEntry {
		Contact contact;
		HashEntryStates info;
		HashEntry() : info(EMPTY) {};
		HashEntry(const Contact& newContact, const HashEntryStates& newInfo) : contact(newContact), info(newInfo) {};
	};
	vector<HashEntry> array;
	int currentSize;
	double loadFactor; //load factor is the ratio of the number of elements in the table to the table size
	/*private methods*/
	//returning the position where search for element terminates, using quadratic probing resolution to avoid the problem of primary clustering.
	int findPosition(const string& firstName, const string& lastName) {
		int probe = 0;
		int currentPos = hash(firstName, lastName);
		while ((array[currentPos].contact.firstName != firstName || array[currentPos].contact.lastName != lastName) && array[currentPos].info != EMPTY) {
			currentPos += (++probe << 1); //shifting probe by 1 bit to the left is sames as multiplying by 2
			//moding by currentSize to avoid overflow
			if (currentPos >= getTableSize())
				currentPos %= getTableSize();
		}
		return currentPos;
	}

	bool isActive(const int& index) {
		return array[index].info == ACTIVE;
	}

	void rehash() {
		vector<HashEntry> oldArray = array;
		//create a new double-sized empty table
		array.resize(nextPrime(2 * oldArray.size()));
		for (HashEntry& entry : array)
			entry.info = EMPTY;
		//set the current size to 0
		currentSize = 0;
		for (HashEntry& entry : oldArray)
			//inserting all the active elements from the old table to the new one
			if (entry.info == ACTIVE)
				insert(entry.contact);
	}
	//hashing function for the contact struct
	int hash(const string& s1, const string& s2) {
		size_t h1 = hash_string(s1);
		size_t h2 = hash_string(s2);

		// Combine the hashes using XOR and bit shifts
		return (h1 ^ (h2 << 1)) % array.size(); //shifting h2 by 1 bit to the left is sames as multiplying by 2
	}

	//custom string hash for fields of the contact struct
	size_t hash_string(const string& s) {
		size_t h = 5381; // Prime number as initial hash value
		for (char c : s) {
			h = (h * 33) ^ c; // Combine hash value with character value using multiplication and XOR
		}
		return h;
	}

	size_t nextPrime(int n) {
		//if n is even, make it odd
		if (n % 2 == 0)
			n++;
		//find the next prime number
		while (!isPrime(n)) {
			n += 2;
		}
		return n;
	}

	bool isPrime(int n) {
		//check if n is a prime number
		if (n == 2 || n == 3)
			return true;
		if (n == 1 || n % 2 == 0)
			return false;
		//check if n is divisible by odd numbers
		for (int i = 3; i * i <= n; i += 2)
			if (n % i == 0)
				return false;
		return true;
	}

	void informUserAfterRehash(const int& prevTableSize, const double& prevLoadFactor) {
		cout << "rehashed..." << endl;
		cout << "previous table size: " << prevTableSize << ", previous load factor: " << prevLoadFactor << ", new table size: " << getTableSize() << ", current unique item count : " << getAllItemSize() << ", current load factor : " << getLoadFactor() << endl;
	}
public:
	//default constructor
	HashTable() :array(INITIAL_TABLE_SIZE), loadFactor(0.5), currentSize(0) {}
	//constructor with custom table size and custom load factor
	HashTable(int size, double loadFactor) :array(size), loadFactor(loadFactor), currentSize(0) {}

	bool remove(const string& firstName, const string& lastName) {
		int currentPos = findPosition(firstName, lastName);
		if (isActive(currentPos)) {
			array[currentPos].info = DELETED;
			currentSize--;
			return true; //if the contact is found and deleted, return true
		}
		return false; //if the contact is not found, return false
	}

	const Contact find(const string& firstName, const string& lastName) {
		int index = findPosition(firstName, lastName);
		if (isActive(index))
			return array[index].contact;
		return Contact(); //if the contact is not found, return an empty contact
	}
	//inserting a new contact to the table
	bool insert(const Contact& newContact) {
		int index = findPosition(newContact.firstName, newContact.lastName);
		if (isActive(index)) {
			return false; // if the contact already exists, return false
		}
		array[index] = HashEntry(newContact, ACTIVE);
		currentSize++;
		//rehashing if the table is half full (load factor = 0.5)
		if (getLoadFactor() >= loadFactor) {
			const int prevTableSize = getTableSize();
			const double prevLoadFactor = getLoadFactor();
			rehash();
			informUserAfterRehash(prevTableSize, prevLoadFactor);
		}
		return true;
	}
	//getters
	double getLoadFactor() {
		return ((double)currentSize / (double)array.size());
	}

	int getTableSize() {
		return array.size();
	}

	int getAllItemSize() {
		return currentSize;
	}


};

//function to convert a string to upper case
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


/*Template class implementation of the Binary search tree*/
template <class C>
class BinarySearchTree {
public:
	explicit BinarySearchTree() :
		root(NULL) {}

	~BinarySearchTree() {
		makeEmpty(root);
	}
	const Contact& findMin() const {
		return findMin(root)->contactInfo;
	}
	const Contact& findMax() const {
		return findMax(root)->contactInfo;
	}
	const Contact find(const string& firstName, const string& lastName) const {
		return searchByFullName(root, firstName, lastName);
	}
	bool isEmpty() const {
		return (root == NULL);
	}

	void makeEmpty() {
		makeEmpty(root);
	}
	bool insert(const Contact& newContact) {
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

private:
	struct Node {
		C contactInfo;
		Node* left;
		Node* right;
		Node(const Contact& contact,
			Node* lt, Node* rt)
			: contactInfo(contact), left(lt), right(rt) { }
	};
	Node* root;
	void insert(const Contact& newContact, Node*& rt, bool& inserted) {
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
			return;   // Item not found; do nothing
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

	const Contact searchByFullName(Node* const& rt, const string& firstName, const string& lastName) const {
		if (rt == NULL) {  // create a new node at the right place
			return Contact();
		}
		else if (firstName == rt->contactInfo.firstName && lastName == rt->contactInfo.lastName) {
			return rt->contactInfo;
		}
		else if (isFirstAlphabetically((rt->contactInfo.firstName + rt->contactInfo.lastName), (firstName + lastName)))
			return searchByFullName(rt->right, firstName, lastName);
		else
			return searchByFullName(rt->left, firstName, lastName);
	}
	void makeEmpty(Node*& rt) {
		if (rt == NULL)
			return;
		makeEmpty(rt->left);
		makeEmpty(rt->right);
		delete rt;
		rt = NULL;
	}

	int height(Node*& rt) const {
		if (rt == NULL)
			return -1;
		return max(height(rt->left), height(rt->right)) + 1;
	}
};
/*End of the Binary search tree class implementation*/

void makeBST(BinarySearchTree<Contact>& BST, ifstream& input_file) {
	string fname, lname, city, phoneNum;
	while (input_file) {
		input_file >> fname >> lname >> phoneNum >> city;
		if (input_file) {
			Contact newContact(toUpperCase(fname), toUpperCase(lname), phoneNum, city);
			BST.insert(newContact);
		}
	}
}

void makeHashTable(HashTable& myHashTable, ifstream& input_file) {
	string fname, lname, city, phoneNum;
	while (input_file) {
		input_file >> fname >> lname >> phoneNum >> city;
		if (input_file) {
			Contact newContact(toUpperCase(fname), toUpperCase(lname), phoneNum, city);
			myHashTable.insert(newContact);
		}
	}
}


void prompt(BinarySearchTree<Contact>& BST, HashTable& HashTable) {
	int choice;
	do {
		cout << "Choose which action to perform from 1 to 4: " << endl;
		cout << "1 - Search a phonebook contact" << endl;
		cout << "2 - Adding a phonebook contact" << endl;
		cout << "3 - Deleting a phonebook contact" << endl;
		cout << "4 - Press 4 to exit" << endl;
		cin >> choice;
		cout << endl;
		switch (choice) {
		case 1: {
			string input, firstName, lastName;
			cout << "Enter name to search for: ";
			cin.ignore();
			getline(cin, input);
			stringstream ss(input);
			ss >> firstName >> lastName;
			firstName = toUpperCase(firstName);
			lastName = toUpperCase(lastName);
			cout << endl;
			//first BST
			cout << "Searching an item in the phonebook  (BST) . . ." << endl;
			cout << "Phonebook: Searching for: (" << firstName << " " << lastName << ")" << endl;
			cout << "====================================" << endl;
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < K; i++)
			{
				if (i == 0) {
					Contact foundContact = BST.find(firstName, lastName);
					if (!foundContact.firstName.empty()) {
						printContact(foundContact);
					}
					else {
						cout << "Name not found!" << endl;
					}
				}
				else {
					BST.find(firstName, lastName);
				}
			}
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
			cout << "\nBST Search Time: " << (BSTTime.count() / K) / 1000000.0 << "\n\n"; //in milliseconds

			cout << "Searching an item in the phonebook  (HashTable) . . ." << endl;
			cout << "Phonebook: Searching for: (" << firstName << " " << lastName << ")" << endl;
			cout << "====================================" << endl;
			start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < K; i++)
			{
				if (i == 0) {
					Contact foundContact = HashTable.find(firstName, lastName);
					if (!foundContact.firstName.empty()) {
						printContact(foundContact);
					}
					else {
						cout << "Name not found!" << endl;
					}
				}
				else {
					HashTable.find(firstName, lastName);
				}
			}
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
			cout << "\nHash Table Search Time: " << (HTTime.count() / K) / 1000000.0 << "\n";
			cout << "Speed Up: " << ((double)BSTTime.count() / HTTime.count()) << "\n\n";
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
			Contact newContact(firstName, lastName, phoneNumber, city);
			//Add to BST
			cout << "Adding an item to the phonebook (BST) . . ." << endl;
			cout << "====================================" << endl;
			auto start = std::chrono::high_resolution_clock::now();
			bool success = BST.insert(newContact);
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
			if (success)
				cout << "Contact has been added successfully to the BST Tree" << endl;
			else
				cout << "Contact is not added. Duplicate exists in the BST tree." << endl;
			cout << endl << endl;
			//add to HashTable
			cout << "Adding an item to the phonebook (HashTable) . . ." << endl;
			cout << "====================================" << endl;
			start = std::chrono::high_resolution_clock::now();
			success = HashTable.insert(newContact);
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
			if (success)
				cout << "Contact has been added successfully to the HashTable" << endl;
			else
				cout << "Contact is not added. Duplicate exists in the HashTable." << endl;
			cout << endl << endl;
			cout << "Adding a contact to the Binary Tree took " << BSTTime.count() / 1000000.0 << endl;
			cout << "Adding a contact to the HashTable took " << HTTime.count() / 1000000.0 << endl;
			cout << endl << endl;
			break;
		}
		case 3: {
			string fullName, firstName, lastName;
			cout << "Deleting an item from the phonebook . . ." << endl;
			cout << "Enter the full name of the contact to be deleted: ";
			cin.ignore();
			getline(cin, fullName);
			cout << endl << endl;
			stringstream ss(fullName);
			ss >> firstName >> lastName;
			firstName = toUpperCase(firstName);
			lastName = toUpperCase(lastName);
			bool success1, success2;
			auto start = std::chrono::high_resolution_clock::now();
			success1 = BST.remove(firstName, lastName);
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
			success2 = HashTable.remove(firstName, lastName);
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
			if (success1 && success2) {
				cout << "Deleted successfully from both the BST and HashTable . . ." << endl;
			}
			cout << endl << endl;
			cout << "Deletion from the Binary Tree took " << BSTTime.count() / 1000000.0 << endl;
			cout << "Deletion from the HashTable took " << HTTime.count() / 1000000.0 << endl;
			cout << endl << endl;
			break;
		}
		case 4: {
			cout << "Exiting the program..." << endl << endl;
			break;
		}
		}

	} while (choice != 4);
}


void run() {
	string fileName;
	cout << "Enter the file name: ";
	getline(cin, fileName);

	ifstream input_file(fileName);
	if (!input_file.is_open()) {
		cerr << "Error opening file. Please try again." << endl;
		return;
	}
	// Create BST
	BinarySearchTree<Contact> BST;
	cout << "Loading the phonebook into a BST . . ." << endl;
	makeBST(BST, input_file);
	cout << "Loaded the phonebook into a BST." << endl;
	const int heightLeftSubTree = BST.heightLeftSubTree();
	const int heightRightSubTree = BST.heightRightSubTree();
	if (checkTreeBalance(heightLeftSubTree, heightRightSubTree))
		cout << "The BST is balanced";
	else
		cout << "The BST is not balanced";
	cout << endl;
	cout << "The heights of BST are for left: " << heightLeftSubTree << " and right: " << heightRightSubTree << endl;
	// Create HashTable
	input_file.clear();
	input_file.seekg(0, ios::beg);
	cout << "Loading the phonebook into a HashTable . . ." << endl;
	HashTable myHashTable(INITIAL_TABLE_SIZE, lambda); // lambda = 0.7, initial table size = 53
	makeHashTable(myHashTable, input_file);
	input_file.close();
	cout << "Loaded the phonebook into a HashTable." << endl;
	cout << "After preprocessing, the contact count is " << myHashTable.getAllItemSize() << ". Current load ratio is " << myHashTable.getLoadFactor() << endl;
	cout << endl << endl;
	// Prompt user
	prompt(BST, myHashTable);
}


int main() {
	run();
	return 0;
}

