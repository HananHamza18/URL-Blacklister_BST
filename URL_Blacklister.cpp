#include<iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include<chrono>
using namespace std;


//-------------------------------------------------------- URL Nodes -----------------------------------------------------------------

class Node {
private:
			string url;
			string type;
		Node* left;
		Node* right;
public:
	Node(string url, string type) {
		this->url = url;
		this->type = type;
		this->left = NULL;
		this->right = NULL;
	}

	void setUrl(string url) {
		this ->url = url;
	}
	string getUrl() {
		return url;
	}

	void setType(string type) {
		this->type = type;
	}

	string getType() {
		return type;
	}

	Node* getLeft() {
		return left;
	}
	Node* getRight() {
		return right;
	}
	void setLeft(Node* left) {
		this->left = left;
	}
	void setRight(Node* right) {
		this->right = right;
	}

	void display() {
		cout << "URL: " << url << ", Type: " << type << endl;
	}
};


//-------------------------------------------------------- History Node -----------------------------------------------------------------

class HistoryNode {
private:
	string url;
	HistoryNode* next;
	HistoryNode* prev;
public:
	HistoryNode(string url) {
		this->url = url;
		this->next = NULL;
		this->prev = NULL;
	}
	void setUrl(string url) {
		this->url = url;
	}
	string getUrl() {
		return url;
	}
	void setNext(HistoryNode* next) {
		this->next = next;
	}
	HistoryNode* getNext() {
		return next;
	}

	void setPrev(HistoryNode* prev) {
		this->prev = prev;
	}
	HistoryNode* getPrev() {
		return prev;
	}
};

//-------------------------------------------------------- History Class -----------------------------------------------------------------
class History {
	private:
	HistoryNode* head;
	HistoryNode* tail;
public:
	History() {
		head = NULL;
		tail = NULL;
	}
	void insertAtTail(string url) {
		HistoryNode* newNode = new HistoryNode(url);
		if (head == NULL) {
			head = tail = newNode;
		}
		else {
			tail->setNext(newNode);
			newNode->setPrev(tail);
			tail = newNode;
		}
	}

	HistoryNode* search(string url) {
		HistoryNode* current = head;
		while (current != NULL) {
			if (current->getUrl() == url) {
				return current;
			}
			current = current->getNext();
		}
		return NULL;
	}

	void remove(string url) {
		HistoryNode* target = search(url);
		HistoryNode* prevNode;
		HistoryNode* nextNode;
		if (target == NULL) {
			cout << "URL not found in history.\n";
			return;
		}
		prevNode = target->getPrev();
		nextNode = target->getNext();
		if(target == head){
			head = nextNode;
			target->setNext(NULL);
			head->setPrev(NULL);
		}
		else if(target == tail){
			tail = prevNode;
			target->setPrev(NULL);
			tail->setNext(NULL);
		}
		else{
			prevNode->setNext(nextNode);
			nextNode->setPrev(prevNode);
			target->setNext(NULL);
			target->setPrev(NULL);
		}
		delete target;
	}

	void display() {
		HistoryNode* current = head;
		while (current != NULL) {
			cout << current->getUrl() <<endl;
			current = current->getNext();
		}
		cout << endl;
	}

	void clearHistory() {
		HistoryNode* current = head;
		while (current != NULL) {
			HistoryNode* temp = current;
			current = current->getNext();
			delete temp;
		}
		head = tail = NULL;
	}

	void traverse() {
		cout << "Browsing History: ";
		cout << "Press 'N' to move forward and 'B' to move back in history or Q to quit." << endl;
		HistoryNode* current = head;
		if (current != NULL) {
			cout<< current->getUrl() << endl;
		}
		char choice;
		do {
			cout << "Enter choice (N/B/Q): ";
			cin >> choice;
			if (choice == 'N' || choice == 'n') {
				if (current->getNext() != NULL) {
					current = current->getNext();
					cout<< current->getUrl() << endl;
				}
				else {
					cout << "No next URL in history." << endl;
				}
			}
			else if (choice == 'B' || choice == 'b') {
				if (current->getPrev() != NULL) {
					current = current->getPrev();
					cout<< current->getUrl() << endl;
				}
				else {
					cout << "No previous URL in history." << endl;
				}
			}
			else {
				cout << "Invalid choice. Please press 'N' or 'B'." << endl;
			}
		} while (choice != 'Q' && choice != 'q');
	}

	void resetHistory() {
		clearHistory();
		cout << "Browsing history has been reset." << endl;
	}

	void displayHistory() {
		display();
	}
};


//-------------------------------------------------------- URL Blacklister Class -----------------------------------------------------------------

class URLBlacklister {
private:
	Node* root;
public:
	URLBlacklister() {
		root = NULL;
	}
	Node* findParent(string url) {
		if (root == NULL || root->getUrl() == url) {
			return NULL;
		}
		Node* current = root;
		Node* parent = NULL;
		while (current != NULL) {
			if (current->getUrl() == url) {
				return parent;
			}
			parent = current;
			if (url < current->getUrl()) {
				current = current->getLeft();
			}
			else {
				current = current->getRight();
			}
		}
		return NULL;
	}

	void insert(string url, string type) {
		Node* newNode = new Node(url,type);
		if(root == NULL) {
			root = newNode;
			return;
		}
		Node* current = root;
		Node* parent = NULL;
		while (true) {
			parent = current;
			if (url == current->getUrl()) {
				delete newNode;
				return;
			}
			else if (url < current->getUrl()) {
				current = current->getLeft();
				if (current == NULL) {
					parent->setLeft(newNode);
					return;
				}
			}
			else {
				current = current->getRight();
				if (current == NULL) {
					parent->setRight(newNode);
					return;
				}
			}
		}
	}

	Node* search(string url) {
		Node* current = root;
		while (current != NULL) {
			if (current->getUrl() == url) {
				return current;
			}
			else if (url < current->getUrl()) {
				current = current->getLeft();
			}
			else {
				current = current->getRight();
			}
		}
		return NULL; // Value not found
	}

	void remove(string url) {
		Node* target = search(url);
		if (target == NULL) {
			cout << "Value " << url << " not found in the tree." << endl;
			return;
		}
		else {
			Node* parent = findParent(url);
			if (target->getLeft() == NULL && target->getRight() == NULL) {
				// Case 1: No children
				if (parent == NULL) {
					root = NULL; // Tree had only one node
				}
				else if (parent->getLeft() == target) {
					parent->setLeft(NULL);
				}
				else {
					parent->setRight(NULL);
				}
				delete target;
			}

			else if (target->getLeft() != NULL && target->getRight() != NULL) {
				// Case 3: Two children
				Node* successorParent = target;
				Node* successor = target->getRight();
				while (successor->getLeft() != NULL) {
					successorParent = successor;
					successor = successor->getLeft();
				}
				target->setUrl(successor->getUrl());
				target->setType(successor->getType());
				if (successorParent->getLeft() == successor) {
					successorParent->setLeft(successor->getRight());
				}
				else {
					successorParent->setRight(successor->getRight());
				}
				delete successor;
			}
			else {
				// Case 2: One child
				Node* child;

				if (target->getLeft() != NULL) {
					child = target->getLeft();
				}
				else {
					child = target->getRight();
				}
				if (parent == NULL) {
					root = child; // Target is root
				}
				else if (parent->getLeft() == target) {
					parent->setLeft(child);
				}
				else {
					parent->setRight(child);
				}
				delete target;
			}
		}
	}



	void displayInOrder(Node* node) {
		if (node != NULL) {
			displayInOrder(node->getLeft());
			node->display();
			displayInOrder(node->getRight());
		}
	}


	void displayBlacklistedURLs() {
		cout << "Blacklisted URLs:\n";
		displayInOrder(root);
	}


	void accessURL(string url, History& history) {
		Node* result = search(url);
		if (result != NULL) {
			cout << "WARNING: BLACKLISTED URL" << endl;
			cout << "Type: " << result->getType() << endl;
			cout << "Access attempt logged for: " << url << endl;
			history.insertAtTail("[BLOCKED] " + url);
		}
		else {
			cout << "Access Granted: " << url << endl;
			history.insertAtTail(url);
		}
	}

	void loadFromCSV(string filename) {
		ifstream file(filename);

		if (!file.is_open()) {
			cout << "Failed to open CSV file.\n";
			return;
		}

		string line;
		getline(file, line); // skip header line

		while (getline(file, line)) {
			stringstream ss(line);
			string url, type;

			getline(ss, url, ',');
			getline(ss, type, ',');

			insert(url, type);
		}

		file.close();
		cout << "Blacklist loaded from CSV successfully.\n";
	}

};

//---------------------------------------------- Main Implementation -----------------------------------------------------------------------

int main() {
	cout << R"(
 _   _______ _      ______ _            _    _ _     _            
| | | | ___ \ |     | ___ \ |          | |  | (_)   | |           
| | | | |_/ / |     | |_/ / | __ _  ___| | _| |_ ___| |_ ___ _ __ 
| | | |    /| |     | ___ \ |/ _` |/ __| |/ / | / __| __/ _ \ '__|
| |_| | |\ \| |____ | |_/ / | (_| | (__|   <| | \__ \ ||  __/ |   
 \___/\_| \_\_____/ \____/|_|\__,_|\___|_|\_\_|_|___/\__\___|_|   

                                                                                )" << endl;
	URLBlacklister blacklister;
	History history;
	blacklister.loadFromCSV("sample.csv");
	int choice;
	do {
		cout << "\nMenu:\n";
		cout << "1. Access URL\n";
		cout << "2. Remove URL from Blacklist\n";
		cout << "3. Display Browsing History\n";
		cout << "4. Traverse Browsing History\n";
		cout << "5. Reset Browsing History\n";
		cout << "6.Display Blacklisted URLs\n";
		cout << "7. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;
		string url;
		switch (choice) {
			case 1:
			cout << "Enter URL to access: ";
			cin.ignore();
			getline(cin, url);
			blacklister.accessURL(url, history);
			system("pause");
			system("cls");
			break;
			case 2:
				cout << "Enter URL to remove from blacklist: ";
				cin.ignore();
				getline(cin, url);
				blacklister.remove(url);
				system("pause");
				system("cls");
				break;
			case 3:
				history.displayHistory();
				system("pause");
				system("cls");
				break;
			case 4:
				history.traverse();
				system("pause");
				system("cls");
				break;
			case 5:
				history.resetHistory();
				system("pause");
				system("cls");
				break;
			case 6:
				blacklister.displayBlacklistedURLs();
				system("pause");
				system("cls");
				break;
			case 7:
				cout << "Exiting program.\n";
				break;
			default:
				cout << "Invalid choice. Please try again.\n";
				break;

		}
	} while (choice != 7);
	
}

