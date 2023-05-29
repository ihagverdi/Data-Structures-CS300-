/*
Author: Hagverdi Ibrahimli
Date: May 28, 2023
Purpose of the program: to sort the contacts in the address book and search for a contact.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

//contact struct
struct Contact {
    string firstName;
    string lastName;
    string phoneNumber;
    string city;
    Contact() {}; //default constructor

    Contact(string _firstName, string _lastName, string _phoneNum, string _city)
        : firstName(_firstName), lastName(_lastName), phoneNumber(_phoneNum), city(_city) {};

    Contact(const Contact& other)
        : firstName(other.firstName), lastName(other.lastName),
        phoneNumber(other.phoneNumber), city(other.city) {}
  
    Contact& operator=(const Contact& other) {
        if (this != &other) {
            firstName = other.firstName;
            lastName = other.lastName;
            phoneNumber = other.phoneNumber;
            city = other.city;
        }
        return *this;
    }
    string getFullName() const {
		return firstName + " " + lastName;
	}

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact) { 
        os << contact.firstName << " " << contact.lastName << " " << contact.phoneNumber << " " << contact.city; 
        return os; 
    }

};


//helper functions
void printAllContacts(const vector<Contact> &contacts) {
    for (const Contact& contact : contacts) {
		cout << contact << endl;
	}
}
bool isSmaller(const string& string1, const string& string2) {
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

void toUpperCase(string& my_string) {
    for (char& c : my_string) {
        c = toupper(c);
    }
}

void swap(Contact &contact1, Contact &contact2) {
    Contact temp = contact1;
    contact1 = contact2;
    contact2 = temp;
}

template<class T>
class InsertionSort {
public:

    void insertionSort(vector<T>& myVector) {
        insertionSort(myVector, 0, myVector.size() - 1);
    }

    void insertionSort(vector<T>& myVector, int left, int right) {
        for (int i = left + 1; i <= right; i++) {
            T temp = myVector[i];
            int j = i - 1;
            while (j >= left && isSmaller(temp.getFullName(), myVector[j].getFullName())) {
                myVector[j + 1] = myVector[j];
                j--;
            }
            myVector[j + 1] = temp;
        }
    }
};

template<class T>
class QuickSort {
public:
    void quickSort(vector<T>& myVector) {
        quickSort(myVector, 0, myVector.size() - 1);
    }
private:
    Contact medianOfThree(vector<T>& myVector, int left, int right) {
        int middle = (left + right) / 2;
        if (isSmaller(myVector[middle].getFullName(), myVector[left].getFullName())) {
            swap(myVector[left], myVector[middle]);
        }
        if (isSmaller(myVector[right].getFullName(), myVector[left].getFullName())) {
            swap(myVector[left], myVector[right]);
        }
        if (isSmaller(myVector[right].getFullName(), myVector[middle].getFullName())) {
            swap(myVector[middle], myVector[right]);
        }
        swap(myVector[middle], myVector[right - 1]);
        return myVector[right - 1];
    }

    void quickSort(vector<T>& myVector, int left, int right) {
        if (left + 10 <= right) {
            T pivot = medianOfThree(myVector, left, right);
            //begin partitioning
            int i = left;
            int j = right - 1;
            while (true) {
                while (isSmaller(myVector[++i].getFullName(), pivot.getFullName())) {}
                while (isSmaller(pivot.getFullName(), myVector[--j].getFullName())) {}
                if (i < j) {
                    swap(myVector[i], myVector[j]);
                }
                else {
                    break;
                }
            }
            swap(myVector[i], myVector[right - 1]);
            quickSort(myVector, left, j);
            quickSort(myVector, i, right);
        }
        else {
            InsertionSort<T>().insertionSort(myVector, left, right);
        }
    }
};

template<class T>
class HeapSort {
public:
    void heapSort(vector<T>& myVector) {
        for (int i = myVector.size() / 2 - 1; i >= 0; i--) {
            percDown(myVector, i, myVector.size());
        }
        for (int j = myVector.size() - 1; j > 0; j--) {
            swap(myVector[0], myVector[j]);
            percDown(myVector, 0, j);
        }
    }
private:
    int leftChild(int i) {
        return 2 * i + 1;
    }
    void percDown(vector<T>& myVector, int i, int n) {
        int child;
        T temp;
        for (temp = myVector[i]; leftChild(i) < n; i = child) {
            child = 2 * i + 1;
            if (child != n - 1 && isSmaller(myVector[child].getFullName(), myVector[child + 1].getFullName())) {
                child++;
            }
            if (isSmaller(temp.getFullName(), myVector[child].getFullName())) {
                myVector[i] = myVector[child];
            }
            else {
                break;
            }
        }
        myVector[i] = temp;
    }
   
};

template<class T>
class MergeSort {
public:
    void mergeSort(vector<T>& myVector) {
        mergeSort(myVector, 0, myVector.size() - 1);
    }
private:
    void merge(vector<T>& myVector, int left, int mid, int right) {
        int i = left;
        int j = mid + 1;

        while (i <= mid && j <= right) {
            if (isSmaller(myVector[i].getFullName(), myVector[j].getFullName())) {
                i++;
            }
            else {
                T temp = myVector[j];
                for (int k = j; k > i; k--) {
                    myVector[k] = myVector[k - 1];
                }
                myVector[i] = temp;
                i++;
                mid++;
                j++;
            }
        }
    }

    void mergeSort(vector<T>& myVector, int left, int right) {
        if (left >= right)
            return;
        int middle = (left + right) / 2;
        mergeSort(myVector, left, middle);
        mergeSort(myVector, middle + 1, right);
        merge(myVector, left, middle, right);
    }
    
};

vector<Contact> searchSequential(const vector<Contact>& contacts, const string& firstName, const string& lastName) {
	vector<Contact> results;
    if (lastName.empty()) {
        //search by first input and return all matches
        for (int i = 0; i < contacts.size(); i++) {
            if (contacts[i].firstName.find(firstName) == 0) {
				results.push_back(contacts[i]);
			}
		}
    }
    else {
        //user provided both first and last name, find if there is a match
        string fullName = firstName + " " + lastName;
        for (int i = 0; i < contacts.size(); i++) {
            if (contacts[i].getFullName() == fullName) {
                results.push_back(contacts[i]);
                break;
            }
        }
    }
    return results;
}

vector<Contact> searchBinary(const vector<Contact>& contacts, const string& firstName, const string& lastName) {
    	vector<Contact> results;
        if (lastName.empty()) {
		//search by first input and return all matches
		int left = 0;
		int right = contacts.size() - 1;
        while (left <= right) {
			int middle = (left + right) / 2;
            if (contacts[middle].firstName.find(firstName) == 0) {
                //check if there are more matches to the left that are smaller alphabetically
                if (middle - 1 >= 0 && contacts[middle - 1].firstName.find(firstName) == 0) {
					right = middle - 1;
				}
                
                else {
					//we found a match and it is the smallest one (alphabetically)
                    for (int i = middle; i < contacts.size(); i++) {
                        if (contacts[i].firstName.find(firstName) == 0) {
							results.push_back(contacts[i]);
						}
                        else {
							break;
						}
					}
                    break;
				}
        
			}
            else if (isSmaller(contacts[middle].firstName, firstName)) {
				left = middle + 1;
			}
            else {
				right = middle - 1;
			}
		}
	}
        else {
		//user provided both first and last name, find if there is a match
		int left = 0;
		int right = contacts.size() - 1;
        string fullName = firstName + " " + lastName;
        while (left <= right) {
			int middle = (left + right) / 2;
            if (contacts[middle].getFullName() == fullName) {
				results.push_back(contacts[middle]);
				break;
			}
            else if (isSmaller(contacts[middle].getFullName(), fullName)) {
				left = middle + 1;
			}
            else {
				right = middle - 1;
			}
		}
	}
	return results; 
}


void run() {
    string fileName;
    cout << "Enter the contact file name: ";
    getline(cin, fileName);

    ifstream input_file(fileName);
    if (!input_file.is_open()) {
        cerr << "Error opening file. Please try again." << endl;
        return;
    }
    //create the vectors for each sorting algorithm
    vector<Contact> contactsQuickSort;
    vector<Contact> contactsMergeSort;
    vector<Contact> contactsInsertionSort;
    vector<Contact> contactsHeapSort;

    string fname, lname, city, phoneNum;
    while (input_file) {
        input_file >> fname >> lname >> phoneNum >> city;
        if (input_file) {
            toUpperCase(fname);
            toUpperCase(lname);
            Contact newContact(fname, lname, phoneNum, city);

            contactsQuickSort.push_back(newContact);
            contactsMergeSort.push_back(newContact);
            contactsInsertionSort.push_back(newContact);
            contactsHeapSort.push_back(newContact);
        }
    }
    input_file.close();

    cout << "Please enter the word to be queried: " << endl;
    string input, firstName, lastName;
    getline(cin, input);
    stringstream ss(input);
    ss >> firstName >> lastName;

    toUpperCase(firstName);
    toUpperCase(lastName);

    auto startTime = std::chrono::high_resolution_clock::now();
    QuickSort<Contact>().quickSort(contactsQuickSort);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto durationQuickSort = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    startTime = std::chrono::high_resolution_clock::now();
    MergeSort<Contact>().mergeSort(contactsMergeSort);
    endTime = std::chrono::high_resolution_clock::now();
    auto durationMergeSort = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    startTime = std::chrono::high_resolution_clock::now();
    InsertionSort<Contact>().insertionSort(contactsInsertionSort);
    endTime = std::chrono::high_resolution_clock::now();
    auto durationInsertionSort = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    startTime = std::chrono::high_resolution_clock::now();
    HeapSort<Contact>().heapSort(contactsHeapSort);
    endTime = std::chrono::high_resolution_clock::now();
    auto durationHeapSort = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    cout << "Sorting the vector copies" << endl;
    cout << "======================================" << endl;
    cout << "Quick Sort Time: " << (durationQuickSort.count()) << " Nanoseconds" << endl;
    cout << "Insertion Sort Time: " << (durationInsertionSort.count()) << " Nanoseconds" << endl;
    cout << "Merge Sort Time: " << (durationMergeSort.count()) << " Nanoseconds" << endl;
    cout << "Heap Sort Time: " <<(durationHeapSort.count()) << " Nanoseconds" << endl;
    cout << endl << endl;

    cout << "Searching for " << firstName << " " << lastName << endl;
    cout << "======================================" << endl;
    
    vector<Contact> results; //vector to store the results of the search
    results = searchBinary(contactsQuickSort, firstName, lastName);
    int N = 100;
    startTime = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= N; i++) {
         searchBinary(contactsQuickSort, firstName, lastName);
    }
    endTime = std::chrono::high_resolution_clock::now();
    auto durationBinarySearch = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    cout << "Search results for Binary Search: " << endl;
    if (results.size() == 0) {
        cout << firstName << " " << lastName << " does not exist in the dataset" << endl;
	}
    else {
        printAllContacts(results);
	}
	cout << "Binary Search Time: " << static_cast<double>(durationBinarySearch.count()) / N  << " Nanoseconds" << endl;
	cout << endl << endl;   

	results = searchSequential(contactsQuickSort, firstName, lastName);
    startTime = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= N; i++) {
        searchSequential(contactsQuickSort, firstName, lastName);
    }
	endTime = std::chrono::high_resolution_clock::now();
	auto durationLinearSearch = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
	cout << "Search results for Sequential Search: " << endl;
    if (results.size() == 0) {  
		cout << firstName << " " << lastName << " does not exist in the dataset" << endl;
    }
    else {
        printAllContacts(results);
    }
    cout << "Sequential Search Time: " << static_cast<double>(durationLinearSearch.count()) / N  << " Nanoseconds" << endl;
    cout << endl << endl;

    cout << "SpeedUp between Search Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << static_cast<double>(durationLinearSearch.count()) / durationBinarySearch.count()  << endl;
    cout << endl << endl;

    cout << "SpeedUps between Sorting Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << static_cast<double>(durationInsertionSort.count()) / durationQuickSort.count()   << endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << static_cast<double>(durationMergeSort.count()) / durationQuickSort.count()  << endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << static_cast<double>(durationHeapSort.count()) / durationQuickSort.count() << endl;
    cout << endl << endl;

}

int main() {
    run();
    return 0;
}
