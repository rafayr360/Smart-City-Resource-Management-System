#include <iostream>
#include <string>
#include <climits>
#include <fstream>
#include <iomanip>
#include <sstream>

// Common Utilities
using namespace std;

// ==================== INPUT VALIDATION FUNCTIONS ====================
// These functions handle user input safely

void clearInputErrors() {
    cin.clear();
    cin.ignore(10000, '\n');
}

string getValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) return input;
        cout << "Input cannot be empty. Please try again.\n";
    }
}

int getValidInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (cin.peek() != '\n') {
                cout << "Invalid input. Please enter a number only.\n";
                clearInputErrors();
                continue;
            }
            if (value >= min && value <= max) {
                cin.ignore();
                return value;
            } else {
                cout << "Please enter a number between " << min << " and " << max << ".\n";
                cin.ignore();
            }
        } else {
            cout << "Invalid input. Please enter a valid number.\n";
            clearInputErrors();
        }
    }
}

void pressEnterToContinue() {
    cout << "\nPress Enter to return to menu...";
    cin.get();
}

// ==================== BASIC DATA STRUCTURES ====================
// Node structure for linked lists, stacks, and queues

template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

// Linked List implementation
template <typename T>
class LinkedList {
public:
    Node<T>* head;
    LinkedList() : head(nullptr) {}

    void push_back(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (!head) {
            head = newNode;
            return;
        }
        Node<T>* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }

    void remove(T val) {
        if (!head) return;
        if (head->data == val) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node<T>* temp = head;
        while (temp->next && !(temp->next->data == val)) {
            temp = temp->next;
        }
        if (temp->next) {
            Node<T>* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
        }
    }

    bool contains(T val) {
        Node<T>* temp = head;
        while (temp) {
            if (temp->data == val) return true;
            temp = temp->next;
        }
        return false;
    }

    ~LinkedList() {
        Node<T>* temp = head;
        while (temp) {
            Node<T>* next = temp->next;
            delete temp;
            temp = next;
        }
    }
};

// Stack implementation using linked list
template <typename T>
class Stack {
    Node<T>* topNode;
public:
    Stack() : topNode(nullptr) {}
    void push(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = topNode;
        topNode = newNode;
    }
    void pop() {
        if (topNode) {
            Node<T>* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }
    T top() {
        if (topNode) return topNode->data;
        return T();
    }
    bool empty() { return topNode == nullptr; }
};

// Queue implementation using linked list
template <typename T>
class Queue {
    Node<T>* frontNode;
    Node<T>* rearNode;
public:
    Queue() : frontNode(nullptr), rearNode(nullptr) {}
    void push(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (!rearNode) {
            frontNode = rearNode = newNode;
            return;
        }
        rearNode->next = newNode;
        rearNode = newNode;
    }
    void pop() {
        if (!frontNode) return;
        Node<T>* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
    }
    T front() {
        if (frontNode) return frontNode->data;
        return T();
    }
    bool empty() { return frontNode == nullptr; }
};

// Dynamic array implementation (used only where necessary)
template <typename T>
class MyVector {
    T* arr;
    int capacity;
    int current;
public:
    MyVector() { arr = new T[10]; capacity = 10; current = 0; }
    void push_back(T data) {
        if (current == capacity) {
            T* temp = new T[2 * capacity];
            for (int i = 0; i < capacity; i++) temp[i] = arr[i];
            delete[] arr;
            capacity *= 2;
            arr = temp;
        }
        arr[current++] = data;
    }
    void pop_back() { if (current > 0) current--; }
    T& operator[](int index) { return arr[index]; }
    int size() { return current; }
    bool empty() { return current == 0; }
    T* begin() { return arr; }
    T* end() { return arr + current; }
    void clear() { current = 0; }
    ~MyVector() { delete[] arr; }
};

// ==================== PAIR STRUCTURE ====================
// Used for Dijkstra's algorithm (distance, location)

template <typename K, typename V>
struct Pair {
    K first;
    V second;
    Pair(K f = K(), V s = V()) : first(f), second(s) {}
    bool operator>(const Pair& other) const { return first > other.first; }
    bool operator<(const Pair& other) const { return first < other.first; }
    bool operator==(const Pair& other) const { return first == other.first; }
};

// ==================== MIN HEAP ====================
// Priority queue implementation for emergency requests and Dijkstra

template <typename T>
class MyMinHeap {
    MyVector<T> heap;

    void heapifyUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2;
        if (heap[parent] > heap[index]) {
            swap(heap[parent], heap[index]);
            heapifyUp(parent);
        }
    }
    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        if (left < heap.size() && heap[left] < heap[smallest]) smallest = left;
        if (right < heap.size() && heap[right] < heap[smallest]) smallest = right;
        if (smallest != index) {
            swap(heap[smallest], heap[index]);
            heapifyDown(smallest);
        }
    }
public:
    void push(T val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }
    void pop() {
        if (heap.empty()) return;
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        heapifyDown(0);
    }
    T top() { return heap[0]; }
    bool empty() { return heap.empty(); }
    int size() { return heap.size(); }
    void clear() { heap.clear(); }

    // Get element at index (for displaying all)
    T getAt(int i) {
        if(i >= 0 && i < heap.size()) return heap[i];
        return T();
    }

    // Get all elements in priority order (creates temporary copy)
    MyVector<T> getAllSorted() {
        MyVector<T> result;
        MyMinHeap<T> tempHeap;

        // Copy all elements to temp heap
        for(int i = 0; i < heap.size(); i++) {
            tempHeap.push(heap[i]);
        }

        // Extract in sorted order
        while(!tempHeap.empty()) {
            result.push_back(tempHeap.top());
            tempHeap.pop();
        }

        return result;
    }
};

// ==================== BINARY SEARCH TREE ====================
// Used for resource management

struct BSTNode {
    int id;
    string name;
    string type;
    BSTNode *left, *right;
    BSTNode(int i, string n, string t) : id(i), name(n), type(t), left(nullptr), right(nullptr) {}
};

class BST {
    BSTNode* root;
    BSTNode* insert(BSTNode* node, int id, string name, string type) {
        if (!node) return new BSTNode(id, name, type);
        if (id < node->id) node->left = insert(node->left, id, name, type);
        else if (id > node->id) node->right = insert(node->right, id, name, type);
        return node;
    }
    BSTNode* search(BSTNode* node, int id) {
        if (!node || node->id == id) return node;
        if (id < node->id) return search(node->left, id);
        return search(node->right, id);
    }
    BSTNode* remove(BSTNode* node, int id) {
        if (!node) return node;
        if (id < node->id) node->left = remove(node->left, id);
        else if (id > node->id) node->right = remove(node->right, id);
        else {
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            BSTNode* temp = minValueNode(node->right);
            node->id = temp->id; node->name = temp->name; node->type = temp->type;
            node->right = remove(node->right, temp->id);
        }
        return node;
    }
    BSTNode* minValueNode(BSTNode* node) {
        BSTNode* current = node;
        while (current && current->left != nullptr) current = current->left;
        return current;
    }
    void inorder(BSTNode* node) {
        if (!node) return;
        inorder(node->left);
        // Show ID and Name
        cout << "| " << setw(5) << node->id << " | " << setw(30) << node->name << " |\n";
        inorder(node->right);
    }
    void saveToFile(BSTNode* node, ofstream& out) {
        if (!node) return;
        if (!out.good()) return;  // Check if file stream is valid

        try {
            out << node->id << "|" << node->name << "|" << node->type << endl;
            if (node->left) saveToFile(node->left, out);
            if (node->right) saveToFile(node->right, out);
        }
        catch(...) {
            // Prevent crash, just return
            return;
        }
    }
    void destroy(BSTNode* node) {
        if(!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
public:
    BST() : root(nullptr) {}
    void add(int id, string name, string type) { root = insert(root, id, name, type); }
    void deleteNode(int id) { root = remove(root, id); }
    bool find(int id) { return search(root, id) != nullptr; }
    BSTNode* retrieve(int id) { return search(root, id); }
    void display() {
        if(!root) { cout << "No resources found.\n"; return; }
        cout << "----------------------------------------\n";
        cout << "| " << setw(5) << "ID" << " | " << setw(30) << "Name" << " |\n";
        cout << "----------------------------------------\n";
        inorder(root);
        cout << "----------------------------------------\n";
    }
    void clear() { destroy(root); root = nullptr; }
    void save(ofstream& out) { saveToFile(root, out); }
};

// ==================== HASH TABLE ====================
// Used for citizen management

struct HashNode {
    int id;
    string name;
    string location;
    HashNode* next;
    HashNode(int i, string n, string l) : id(i), name(n), location(l), next(nullptr) {}
};

class HashTable {
    static const int TABLE_SIZE = 100;
    HashNode* table[TABLE_SIZE];
    int hashFunc(int id) { return id % TABLE_SIZE; }
public:
    HashTable() { for(int i=0; i<TABLE_SIZE; i++) table[i] = nullptr; }

    void add(int id, string name, string location) {
        int idx = hashFunc(id);
        HashNode* newNode = new HashNode(id, name, location);
        newNode->next = table[idx];
        table[idx] = newNode;
    }

    bool remove(int id) {
        int idx = hashFunc(id);
        HashNode* curr = table[idx];
        HashNode* prev = nullptr;
        while(curr) {
            if(curr->id == id) {
                if(prev) prev->next = curr->next;
                else table[idx] = curr->next;
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    bool exists(int id) {
        int idx = hashFunc(id);
        HashNode* curr = table[idx];
        while(curr) {
            if(curr->id == id) return true;
            curr = curr->next;
        }
        return false;
    }

    void search(int id) {
        int idx = hashFunc(id);
        HashNode* curr = table[idx];
        while(curr) {
            if(curr->id == id) {
                cout << "Citizen Found: ID " << curr->id << ", Name: " << curr->name << ", Loc: " << curr->location << endl;
                return;
            }
            curr = curr->next;
        }
        cout << "Citizen not found.\n";
    }

    // Get citizen location by ID
    string getLocation(int id) {
        int idx = hashFunc(id);
        HashNode* curr = table[idx];
        while(curr) {
            if(curr->id == id) return curr->location;
            curr = curr->next;
        }
        return "";
    }

    // Get citizen name by ID
    string getCitizenName(int id) {
        int idx = hashFunc(id);
        HashNode* curr = table[idx];
        while(curr) {
            if(curr->id == id) return curr->name;
            curr = curr->next;
        }
        return "Unknown";
    }

    // Update citizen location
    void updateLocation(int id, string newLoc) {
        int idx = hashFunc(id);
        HashNode* curr = table[idx];
        while(curr) {
            if(curr->id == id) {
                curr->location = newLoc;
                return;
            }
            curr = curr->next;
        }
    }

    void displayAll() {
        cout << "--------------------------------------------------\n";
        cout << "| " << setw(5) << "ID" << " | " << setw(20) << "Name" << " | " << setw(15) << "Location" << " |\n";
        cout << "--------------------------------------------------\n";
        bool empty = true;
        for(int i=0; i<TABLE_SIZE; i++) {
            HashNode* curr = table[i];
            while(curr) {
                cout << "| " << setw(5) << curr->id << " | " << setw(20) << curr->name << " | " << setw(15) << curr->location << " |\n";
                curr = curr->next;
                empty = false;
            }
        }
        if(empty) cout << "| " << setw(46) << "No citizens found." << " |\n";
        cout << "--------------------------------------------------\n";
    }

    void clear() {
        for(int i=0; i<TABLE_SIZE; i++) {
             while(table[i]) {
                 HashNode* temp = table[i];
                 table[i] = table[i]->next;
                 delete temp;
             }
        }
    }

    // Relocate all citizens from one location to another
    void relocateCitizens(string fromLoc, string toLoc) {
        for(int i=0; i<TABLE_SIZE; i++) {
            HashNode* curr = table[i];
            while(curr) {
                if(curr->location == fromLoc) {
                    curr->location = toLoc;
                    cout << "Citizen ID " << curr->id << " relocated to " << toLoc << endl;
                }
                curr = curr->next;
            }
        }
    }

    void save(ofstream& out) {
        if(!out.good()) return;

        for(int i=0; i<TABLE_SIZE; i++) {
            HashNode* curr = table[i];
            while(curr) {
                if(out.good()) {
                    out << curr->id << "|" << curr->name << "|" << curr->location << endl;
                }
                curr = curr->next;
            }
        }
    }
};

// ==================== GRAPH STRUCTURES ====================

struct RouteEdge {
    string dest;
    int weight;
    bool operator==(const RouteEdge& other) const { return dest == other.dest; }
};

// ==================== EMERGENCY REQUEST STRUCTURE ====================

struct EmergencyReq {
    int id;
    string type;
    string location;
    int priority;
    int arrivalTime;
    int citizenID;

    // Default constructor
    EmergencyReq() : id(0), type(""), location(""), priority(3), arrivalTime(0), citizenID(0) {}

    // Parameterized constructor
    EmergencyReq(int i, string t, string l, int p, int a, int c)
        : id(i), type(t), location(l), priority(p), arrivalTime(a), citizenID(c) {}

    bool operator>(const EmergencyReq& other) const {
        if (priority != other.priority) return priority > other.priority;
        return arrivalTime > other.arrivalTime;
    }
    bool operator<(const EmergencyReq& other) const {
        if (priority != other.priority) return priority < other.priority;
        return arrivalTime < other.arrivalTime;
    }
};

// ==================== SCHEDULED SERVICE STRUCTURE ====================

struct ScheduledService {
    string type;
    string location;
    int priority;

    // Default constructor
    ScheduledService() : type(""), location(""), priority(3) {}

    // Parameterized constructor
    ScheduledService(string t, string l, int p) : type(t), location(l), priority(p) {}

    bool operator>(const ScheduledService& other) const { return priority > other.priority; }
    bool operator<(const ScheduledService& other) const { return priority < other.priority; }
};

// ==================== MAIN SCRMS CLASS ====================

class SCRMS {
private:
    // Graph structure for city map
    struct LocationNode {
        string name;
        LinkedList<RouteEdge> routes;
    };
    MyVector<LocationNode> cityMap;

    // Emergency service queue (priority-based)
    MyMinHeap<EmergencyReq> emergencyQueue;
    int reqCounter = 1;

    // Resource management (BST)
    BST resourceTree;
    int resourceIDCounter = 1;

    // Citizen management (Hash Table)
    HashTable citizenTable;
    int citizenIDCounter = 1;

    // Service scheduling (priority queue)
    MyMinHeap<ScheduledService> serviceQueue;

    // Undo/Redo stacks
    struct Command {
        string action;
        string data;
    };
    Stack<Command> undoStack;
    Stack<Command> redoStack;

    void addUndo(string action, string data) {
        undoStack.push({action, data});
        while(!redoStack.empty()) redoStack.pop();
    }

    // Find location index in city map
    int findLocIndex(string name) {
        for(int i=0; i<cityMap.size(); i++) {
            if(cityMap[i].name == name) return i;
        }
        return -1;
    }

public:
    bool locationExists(string name) {
        return findLocIndex(name) != -1;
    }

    // ==================== CITY MAP MANAGEMENT ====================

    void addLocation(string name) {
        if(findLocIndex(name) != -1) {
            cout << "Location already exists.\n";
            return;
        }
        LocationNode node;
        node.name = name;
        cityMap.push_back(node);
        cout << "Location added: " << name << endl;
        addUndo("ADD_LOC", name);
    }

    void removeLocation(string name) {
        int idx = findLocIndex(name);
        if(idx == -1) {
            cout << "Location not found.\n";
            return;
        }

        // Cascade deletion: Remove all routes pointing to this location
        for(int i=0; i<cityMap.size(); i++) {
            Node<RouteEdge>* temp = cityMap[i].routes.head;
            while(temp) {
                if(temp->data.dest == name) {
                    cityMap[i].routes.remove(temp->data);
                    break;
                }
                temp = temp->next;
            }
        }

        // Remove all emergency requests at this location
        MyMinHeap<EmergencyReq> tempQueue;
        while(!emergencyQueue.empty()) {
            EmergencyReq req = emergencyQueue.top();
            emergencyQueue.pop();
            if(req.location != name) {
                tempQueue.push(req);
            }
        }
        emergencyQueue = tempQueue;

        // Remove all scheduled services at this location
        MyMinHeap<ScheduledService> tempServices;
        while(!serviceQueue.empty()) {
            ScheduledService svc = serviceQueue.top();
            serviceQueue.pop();
            if(svc.location != name) {
                tempServices.push(svc);
            }
        }
        serviceQueue = tempServices;

        // Relocate citizens at this location to nearest location
        string nearestLoc = "";

        // Find nearest location (just pick first available)
        if(cityMap.size() > 1) {
            for(int i=0; i<cityMap.size(); i++) {
                if(cityMap[i].name != name) {
                    nearestLoc = cityMap[i].name;
                    break;
                }
            }
        }

        // Relocate all citizens from deleted location
        if(!nearestLoc.empty()) {
            citizenTable.relocateCitizens(name, nearestLoc);
        }

        // Remove the location itself
        if (idx != cityMap.size() - 1) {
            cityMap[idx] = cityMap[cityMap.size() - 1];
        }
        cityMap.pop_back();
        cout << "Location removed.\n";
        addUndo("REM_LOC", name);
    }

    void viewLocations() {
        if(cityMap.empty()) { cout << "No locations found.\n"; return; }
        cout << "--- Available Locations ---\n";
        for(int i=0; i<cityMap.size(); i++) {
             cout << "ID: " << i << " | Name: " << cityMap[i].name << endl;
        }
    }

    // Add route with validation
    void addRouteInteractive() {
        if(cityMap.empty()) { cout << "No locations to connect.\n"; return; }

        viewLocations();
        cout << "\nEnter Source Location (Name or ID): ";
        string srcInput; getline(cin, srcInput);
        string src = parseLocInput(srcInput);
        if(src == "") return;

        viewLocations();
        cout << "\nEnter Destination Location (Name or ID): ";
        string destInput; getline(cin, destInput);
        string dest = parseLocInput(destInput);
        if(dest == "") return;

        // Prevent same location to same location
        if(src == dest) {
            cout << "Error: Cannot create route from location to itself.\n";
            return;
        }

        int dist = getValidInt("Enter Distance: ", 1, 1000);
        addRoute(src, dest, dist);

        // Remove the automatic message (just add reverse silently)
        addRoute(dest, src, dist);
    }

    string parseLocInput(string input) {
        stringstream ss(input);
        int id;
        if ((ss >> id) && ss.eof()) {
            if(id >= 0 && id < cityMap.size()) return cityMap[id].name;
        }
        if(locationExists(input)) return input;
        cout << "Invalid location.\n";
        return "";
    }

    void addRoute(string src, string dest, int w) {
        int u = findLocIndex(src);
        int v = findLocIndex(dest);
        if(u == -1 || v == -1) return;

        // Update if exists
        Node<RouteEdge>* temp = cityMap[u].routes.head;
        while(temp) {
            if(temp->data.dest == dest) {
                temp->data.weight = w;
                return;
            }
            temp = temp->next;
        }
        cityMap[u].routes.push_back({dest, w});
        addUndo("ADD_ROUTE", src + " " + dest);
    }

    // Remove route bidirectionally
    void removeRouteInteractive() {
        if(cityMap.empty()) { cout << "No locations found.\n"; return; }

        // Display available routes
        cout << "\n--- Available Routes ---\n";
        bool hasRoutes = false;
        for(int i=0; i<cityMap.size(); i++) {
            Node<RouteEdge>* temp = cityMap[i].routes.head;
            while(temp) {
                cout << cityMap[i].name << " -> " << temp->data.dest << " (" << temp->data.weight << " km)\n";
                temp = temp->next;
                hasRoutes = true;
            }
        }
        if(!hasRoutes) {
            cout << "No routes available.\n";
            return;
        }

        cout << "\nEnter Source Location: ";
        string src = getValidString("");
        cout << "Enter Destination Location: ";
        string dest = getValidString("");

        int u = findLocIndex(src);
        int v = findLocIndex(dest);

        if(u == -1 || v == -1) {
            cout << "Invalid location(s).\n";
            return;
        }

        // Remove both directions
        bool removed1 = false, removed2 = false;

        Node<RouteEdge>* temp = cityMap[u].routes.head;
        while(temp) {
            if(temp->data.dest == dest) {
                cityMap[u].routes.remove(temp->data);
                removed1 = true;
                break;
            }
            temp = temp->next;
        }

        temp = cityMap[v].routes.head;
        while(temp) {
            if(temp->data.dest == src) {
                cityMap[v].routes.remove(temp->data);
                removed2 = true;
                break;
            }
            temp = temp->next;
        }

        if(removed1 || removed2) {
            cout << "Route removed (both directions).\n";
        } else {
            cout << "Route not found.\n";
        }
    }

    void displayCityMap() {
        cout << "\n--- City Map ---\n";
        for(int i=0; i<cityMap.size(); i++) {
            cout << left << setw(15) << cityMap[i].name << " -> ";
            Node<RouteEdge>* temp = cityMap[i].routes.head;
            if(!temp) cout << "No routes";
            while(temp) {
                cout << "[" << temp->data.dest << ":" << temp->data.weight << "] ";
                temp = temp->next;
            }
            cout << endl;
        }
    }

    // ==================== EMERGENCY SERVICES ====================

    void addEmergencyRequestFormatted() {
        int citID = getValidInt("Enter Citizen ID: ", 1, 99999);
        if(!citizenTable.exists(citID)) {
            cout << "Error: Citizen ID " << citID << " does not exist. Request denied.\n";
            return;
        }
        string type = getValidString("Enter Request Type (e.g. Fire, Medical): ");
        string loc;
        while(true) {
            loc = getValidString("Enter Location Name: ");
            if(locationExists(loc)) break;

            cout << "Location not found. (1) Add to Map, (2) Retry: ";
            int choice = getValidInt("", 1, 2);
            if(choice == 1) {
                addLocation(loc);
                break;
            }
        }
        int prio = getValidInt("Enter Priority (1-High, 2-Medium, 3-Low): ", 1, 3);

        // Create emergency request with proper values
        int currentReqId = reqCounter;
        emergencyQueue.push(EmergencyReq(currentReqId, type, loc, prio, currentReqId, citID));
        reqCounter++;

        cout << "Request Added Successfully.\n";
        addUndo("ADD_EMERGENCY", type);
    }

    // Show ALL pending requests sorted by priority
    void showPendingRequests() {
        if(emergencyQueue.empty()) { cout << "No pending requests.\n"; return; }

        cout << "\n--- All Pending Emergency Requests (Priority Order) ---\n";
        cout << "--------------------------------------------------------------------------------------------\n";
        cout << "| " << setw(5) << "ID" << " | " << setw(12) << "Type" << " | "
             << setw(15) << "Location" << " | " << setw(8) << "Priority"
             << " | " << setw(10) << "Citizen ID" << " | " << setw(20) << "Citizen Name" << " |\n";
        cout << "--------------------------------------------------------------------------------------------\n";

        // Get all requests in sorted order (safe method)
        MyVector<EmergencyReq> requests = emergencyQueue.getAllSorted();

        for(int i = 0; i < requests.size(); i++) {
            EmergencyReq req = requests[i];
            string prioStr = (req.priority == 1) ? "High" : (req.priority == 2) ? "Medium" : "Low";
            string citizenName = citizenTable.getCitizenName(req.citizenID);

            cout << "| " << setw(5) << req.id << " | " << setw(12) << req.type << " | "
                 << setw(15) << req.location << " | " << setw(8) << prioStr
                 << " | " << setw(10) << req.citizenID << " | " << setw(20) << citizenName << " |\n";
        }
        cout << "--------------------------------------------------------------------------------------------\n";
    }

    void completeNextRequest() {
        if(emergencyQueue.empty()) { cout << "No requests to complete.\n"; return; }
        cout << "Completed Request: " << emergencyQueue.top().type << " for Citizen ID " << emergencyQueue.top().citizenID << endl;
        emergencyQueue.pop();
        addUndo("COMP_EMERGENCY", "req");
    }

    // ==================== RESOURCE MANAGEMENT ====================

    void addResourceAuto() {
        string name = getValidString("Enter Resource Name: ");
        string type = "General";
        resourceTree.add(resourceIDCounter, name, type);
        cout << "Resource Added: " << name << " (ID: " << resourceIDCounter << ")\n";
        addUndo("ADD_RES", name);
        resourceIDCounter++;
    }

    void viewResources() {
        resourceTree.display();
    }

    // ==================== CITIZEN MANAGEMENT ====================

    void addCitizenAuto() {
        string name = getValidString("Enter Name: ");
        string loc = getValidString("Enter Location: ");

        if(!locationExists(loc)) {
            cout << "Location '" << loc << "' not found in map. Adding it now...\n";
            addLocation(loc);
        }

        citizenTable.add(citizenIDCounter, name, loc);
        cout << "Citizen Added: " << name << " (ID: " << citizenIDCounter << ")\n";
        addUndo("ADD_CITIZEN", name);
        citizenIDCounter++;
    }

    // Remove citizen option
    void removeCitizen() {
        // First show all citizens so user can see IDs
        cout << "\n--- Current Citizens ---\n";
        citizenTable.displayAll();

        cout << "\n";
        int id = getValidInt("Enter Citizen ID to remove: ", 1, 99999);
        if(citizenTable.remove(id)) {
            cout << "Citizen ID " << id << " removed successfully.\n";
        } else {
            cout << "Citizen ID " << id << " not found.\n";
        }
    }

    void viewCitizens() {
        citizenTable.displayAll();
    }

    // ==================== SERVICE SCHEDULING ====================

    void scheduleServiceInteractive() {
        string type = getValidString("Enter Service Type: ");
        string loc;
        while(true) {
            loc = getValidString("Enter Location: ");
            if(locationExists(loc)) break;
            cout << "Location not found. (1) Add to Map, (2) Retry: ";
             int choice = getValidInt("", 1, 2);
            if(choice == 1) {
                addLocation(loc);
                break;
            }
        }
        int prio = getValidInt("Enter Priority (1-Urgent, 2-Normal, 3-Routine): ", 1, 3);
        serviceQueue.push({type, loc, prio});
        cout << "Service Scheduled.\n";
    }

    void viewNextService() {
        if(serviceQueue.empty()) {
            cout << "No services.\n";
        } else {
            ScheduledService svc = serviceQueue.top();
            cout << "Next Service: " << svc.type << " at " << svc.location
                 << " (Priority " << svc.priority << ")\n";
        }
    }

    // Complete service option
    void completeNextService() {
        if(serviceQueue.empty()) {
            cout << "No services to complete.\n";
            return;
        }
        cout << "Completed Service: " << serviceQueue.top().type << " at " << serviceQueue.top().location << endl;
        serviceQueue.pop();
    }

    // Show all services
    void showAllServices() {
        if(serviceQueue.empty()) {
            cout << "No scheduled services.\n";
            return;
        }

        cout << "\n--- All Scheduled Services (Priority Order) ---\n";
        cout << "------------------------------------------------------------\n";
        cout << "| " << setw(20) << "Type" << " | " << setw(15) << "Location"
             << " | " << setw(10) << "Priority" << " |\n";
        cout << "------------------------------------------------------------\n";

        // Get all services in sorted order (safe method)
        MyVector<ScheduledService> services = serviceQueue.getAllSorted();

        for(int i = 0; i < services.size(); i++) {
            ScheduledService svc = services[i];
            string prioStr = (svc.priority == 1) ? "Urgent" : (svc.priority == 2) ? "Normal" : "Routine";
            cout << "| " << setw(20) << svc.type << " | " << setw(15) << svc.location
                 << " | " << setw(10) << prioStr << " |\n";
        }
        cout << "------------------------------------------------------------\n";
    }

    // ==================== ROUTE OPTIMIZATION (DIJKSTRA) ====================

    void findShortestPath(string start, string end) {
        int startIdx = findLocIndex(start);
        int endIdx = findLocIndex(end);
        if(startIdx == -1 || endIdx == -1) {
            cout << "Invalid locations.\n";
            return;
        }

        MyVector<int> dist;
        MyVector<string> parent;
        for(int i=0; i<cityMap.size(); i++) {
            dist.push_back(INT_MAX);
            parent.push_back("");
        }

        dist[startIdx] = 0;
        MyMinHeap<Pair<int, string>> pq;
        pq.push({0, start});

        while(!pq.empty()) {
            string uName = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            int uIdx = findLocIndex(uName);
            if(d > dist[uIdx]) continue;

            Node<RouteEdge>* temp = cityMap[uIdx].routes.head;
            while(temp) {
                string vName = temp->data.dest;
                int weight = temp->data.weight;
                int vIdx = findLocIndex(vName);

                if(vIdx != -1 && dist[uIdx] + weight < dist[vIdx]) {
                    dist[vIdx] = dist[uIdx] + weight;
                    parent[vIdx] = uName;
                    pq.push({dist[vIdx], vName});
                }
                temp = temp->next;
            }
        }

        int finalIdx = findLocIndex(end);
        if(dist[finalIdx] == INT_MAX) {
            cout << "No route exists.\n";
        } else {
            // Requirement 9: Show total distance in km
            cout << "Shortest Distance: " << dist[finalIdx] << " km\nPath: ";
            string curr = end;
            MyVector<string> pathStack;
            while(curr != "") {
                pathStack.push_back(curr);
                if(curr == start) break;
                curr = parent[findLocIndex(curr)];
            }
            for(int i=pathStack.size()-1; i>=0; i--) {
                cout << pathStack[i] << (i > 0 ? " -> " : "");
            }
            cout << endl;
        }
    }

    // ==================== GRAPH TRAVERSAL (BFS/DFS) ====================

    void bfsTraversal(string start) {
        int idx = findLocIndex(start);
        if(idx == -1) { cout << "Start invalid.\n"; return; }

        cout << "BFS Traversal: ";
        Queue<int> q;
        MyVector<bool> visited;
        for(int i=0; i<cityMap.size(); i++) visited.push_back(false);

        visited[idx] = true;
        q.push(idx);

        while(!q.empty()) {
            int u = q.front(); q.pop();
            cout << cityMap[u].name << " ";

            Node<RouteEdge>* temp = cityMap[u].routes.head;
            while(temp) {
                int v = findLocIndex(temp->data.dest);
                if(v != -1 && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
                temp = temp->next;
            }
        }
        cout << endl;
    }

    void dfsHelper(int u, MyVector<bool>& visited) {
        visited[u] = true;
        cout << cityMap[u].name << " ";
        Node<RouteEdge>* temp = cityMap[u].routes.head;
        while(temp) {
             int v = findLocIndex(temp->data.dest);
             if(v != -1 && !visited[v]) {
                 dfsHelper(v, visited);
             }
             temp = temp->next;
        }
    }

    void dfsTraversal(string start) {
        int idx = findLocIndex(start);
        if(idx == -1) { cout << "Start invalid.\n"; return; }

        cout << "DFS Traversal: ";
        MyVector<bool> visited;
        for(int i=0; i<cityMap.size(); i++) visited.push_back(false);
        dfsHelper(idx, visited);
        cout << endl;
    }

    // ==================== UNDO/REDO ====================

    void undo() {
        if(undoStack.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        Command cmd = undoStack.top();
        undoStack.pop();
        redoStack.push(cmd);

        // Actually perform the undo operation
        if(cmd.action == "ADD_LOC") {
            int idx = findLocIndex(cmd.data);
            if(idx != -1) {
                if (idx != cityMap.size() - 1) {
                    cityMap[idx] = cityMap[cityMap.size() - 1];
                }
                cityMap.pop_back();
                cout << "Undid: Removed location '" << cmd.data << "'\n";
            }
        }
        else if(cmd.action == "REM_LOC") {
            // Undo remove location - add it back
            LocationNode node;
            node.name = cmd.data;
            cityMap.push_back(node);
            cout << "Undid: Re-added location '" << cmd.data << "'\n";
        }
        else if(cmd.action == "ADD_CITIZEN") {
            cout << "Undid: Added citizen (Note: Citizen data not fully reversed)\n";
        }
        else if(cmd.action == "ADD_RES") {
            cout << "Undid: Added resource (Note: Resource data not fully reversed)\n";
        }
        else {
            cout << "Undid action: " << cmd.action << "\n";
        }
    }

    void redo() {
        if(redoStack.empty()) {
            cout << "Nothing to redo.\n";
            return;
        }

        Command cmd = redoStack.top();
        redoStack.pop();
        undoStack.push(cmd);

        // Actually perform the redo operation
        if(cmd.action == "ADD_LOC") {
            // Redo add location
            LocationNode node;
            node.name = cmd.data;
            cityMap.push_back(node);
            cout << "Redid: Added location '" << cmd.data << "'\n";
        }
        else if(cmd.action == "REM_LOC") {
            // Redo remove location
            int idx = findLocIndex(cmd.data);
            if(idx != -1) {
                if (idx != cityMap.size() - 1) {
                    cityMap[idx] = cityMap[cityMap.size() - 1];
                }
                cityMap.pop_back();
                cout << "Redid: Removed location '" << cmd.data << "'\n";
            }
        }
        else {
            cout << "Redid action: " << cmd.action << "\n";
        }
    }

    // ==================== DATA PERSISTENCE ====================


    void save() {
         ofstream out("scrms_data.txt");
         if(!out) {
             cout << "Error: Cannot open file for saving.\n";
             return;
         }

         try {
             // Save counters
             out << resourceIDCounter << " " << citizenIDCounter << " " << reqCounter << endl;
             if(!out.good()) {
                 cout << "Error writing counters.\n";
                 out.close();
                 return;
             }

             // Save locations and routes
             out << cityMap.size() << endl;
             for(int i=0; i<cityMap.size(); i++) {
                 out << cityMap[i].name << endl;
                 int rCount = 0;
                 Node<RouteEdge>* temp = cityMap[i].routes.head;
                 while(temp) { rCount++; temp=temp->next; }
                 out << rCount << endl;
                 temp = cityMap[i].routes.head;
                 while(temp) {
                     out << temp->data.dest << "|" << temp->data.weight << endl;
                     temp = temp->next;
                 }
             }

             if(!out.good()) {
                 cout << "Error writing locations.\n";
                 out.close();
                 return;
             }

             // Save resources
             out << "RESOURCES" << endl;
             resourceTree.save(out);
             out << "END_RESOURCES" << endl;

             if(!out.good()) {
                 cout << "Error writing resources.\n";
                 out.close();
                 return;
             }

             // Save citizens
             out << "CITIZENS" << endl;
             citizenTable.save(out);
             out << "END_CITIZENS" << endl;

             if(!out.good()) {
                 cout << "Error writing citizens.\n";
                 out.close();
                 return;
             }

             cout << "System data saved to scrms_data.txt successfully.\n";
             out.close();
         }
         catch(...) {
             cout << "Critical error during save. Data may be corrupted.\n";
             out.close();
         }
    }

    void load() {
        ifstream in("scrms_data.txt");
        if(!in) { cout << "No save file found.\n"; return; }

        string line;
        if(in >> resourceIDCounter >> citizenIDCounter >> reqCounter) {
             getline(in, line);

             int mapSize; in >> mapSize; getline(in, line);
             cityMap.clear();

             for(int i=0; i<mapSize; i++) {
                 string name; getline(in, name);
                 LocationNode node; node.name = name;
                 cityMap.push_back(node);

                 int rCount; in >> rCount; getline(in, line);
                 for(int j=0; j<rCount; j++) {
                     string rLine; getline(in, rLine);

                     // Parse without using vector
                     int pipePos = rLine.find('|');
                     string d = rLine.substr(0, pipePos);
                     int w = 0;
                     if(pipePos != string::npos) {
                         w = stoi(rLine.substr(pipePos+1));
                     }
                     cityMap[i].routes.push_back({d, w});
                 }
             }

             // Load resources
             getline(in, line);
             resourceTree.clear();
             while(getline(in, line) && line != "END_RESOURCES") {
                 // Parse without vector
                 int pipe1 = line.find('|');
                 int pipe2 = line.find('|', pipe1 + 1);

                 if(pipe1 != string::npos && pipe2 != string::npos) {
                     int id = stoi(line.substr(0, pipe1));
                     string name = line.substr(pipe1 + 1, pipe2 - pipe1 - 1);
                     string type = line.substr(pipe2 + 1);
                     resourceTree.add(id, name, type);
                 }
             }

             // Load citizens
             getline(in, line);
             citizenTable.clear();
             while(getline(in, line) && line != "END_CITIZENS") {
                 // Parse without vector
                 int pipe1 = line.find('|');
                 int pipe2 = line.find('|', pipe1 + 1);

                 if(pipe1 != string::npos && pipe2 != string::npos) {
                     int id = stoi(line.substr(0, pipe1));
                     string name = line.substr(pipe1 + 1, pipe2 - pipe1 - 1);
                     string location = line.substr(pipe2 + 1);
                     citizenTable.add(id, name, location);
                 }
             }
        }
        cout << "Data loaded successfully.\n";
        in.close();
    }

    void exitSystem() {
        save();
        cout << "Exiting system. Goodbye!\n";
    }
};

// ==================== MENU DISPLAY ====================

void displayMenu() {
    cout << "\n======================================================\n";
    cout << "   SMART CITY RESOURCE MANAGEMENT SYSTEM (SCRMS)\n";
    cout << "======================================================\n";
    cout << "1. Manage City Map (Locations & Routes)\n";
    cout << "2. Emergency Service Requests\n";
    cout << "3. Route Optimization\n";
    cout << "4. Resource Management\n";
    cout << "5. Citizen Management\n";
    cout << "6. Service Scheduling\n";
    cout << "7. Undo/Redo Last Operation\n";
    cout << "8. View City Connectivity (BFS/DFS)\n";
    cout << "9. Data Persistence (Save/Load)\n";
    cout << "10. Exit Application\n";
    cout << "======================================================\n";
    cout << "Select an option (1-10): ";
}

// ==================== MAIN FUNCTION ====================

int main() {
    SCRMS system;

    int choice;
    do {
        displayMenu();
        choice = getValidInt("", 1, 10);

        switch(choice) {
            case 1: {
                int sub;
                do {
                    cout << "\n--- Manage City Map ---\n";
                    cout << "1. Add a New Location\n";
                    cout << "2. Add a Route (Bidirectional)\n";
                    cout << "3. Remove a Location\n";
                    cout << "4. Remove a Route\n";
                    cout << "5. View City Map\n";
                    cout << "6. Back to Main Menu\n";
                    sub = getValidInt("Enter option: ", 1, 6);
                    if(sub==1) system.addLocation(getValidString("Enter Location Name: "));
                    else if(sub==2) system.addRouteInteractive();
                    else if(sub==3) {
                        system.viewLocations();
                        system.removeLocation(getValidString("Enter Location to Remove: "));
                    }
                    else if(sub==4) system.removeRouteInteractive();
                    else if(sub==5) system.displayCityMap();
                } while(sub != 6);
                break;
            }
            case 2: {
                int sub;
                do {
                    cout << "\n--- Emergency Services ---\n";
                    cout << "1. Add Emergency Request\n";
                    cout << "2. View Pending Requests\n";
                    cout << "3. Complete Next Request\n";
                    cout << "4. Back\n";
                    sub = getValidInt("Enter option: ", 1, 4);
                    if(sub==1) system.addEmergencyRequestFormatted();
                    else if(sub==2) system.showPendingRequests();
                    else if(sub==3) system.completeNextRequest();
                } while(sub != 4);
                break;
            }
            case 3:
                cout << "\n--- Route Optimization ---\n";
                system.findShortestPath(getValidString("Start: "), getValidString("End: "));
                pressEnterToContinue();
                break;
            case 4: {
                int sub;
                do {
                    cout << "\n--- Resource Management ---\n";
                    cout << "1. Add Resource\n";
                    cout << "2. View All Resources\n";
                    cout << "3. Back\n";
                    sub = getValidInt("Enter option: ", 1, 3);
                    if(sub==1) system.addResourceAuto();
                    else if(sub==2) system.viewResources();
                } while(sub != 3);
                break;
            }
            case 5: {
                int sub;
                do {
                    cout << "\n--- Citizen Management ---\n";
                    cout << "1. Add Citizen\n";
                    cout << "2. Remove Citizen\n";
                    cout << "3. View All Citizens\n";
                    cout << "4. Back\n";
                    sub = getValidInt("Enter option: ", 1, 4);
                    if(sub==1) system.addCitizenAuto();
                    else if(sub==2) system.removeCitizen();
                    else if(sub==3) system.viewCitizens();
                } while(sub != 4);
                break;
            }
            case 6: {
                int sub;
                do {
                    cout << "\n--- Service Scheduling ---\n";
                    cout << "1. Schedule New Service\n";
                    cout << "2. View Next Service\n";
                    cout << "3. Complete Next Service\n";
                    cout << "4. View All Services\n";
                    cout << "5. Back\n";
                    sub = getValidInt("Enter option: ", 1, 5);
                    if(sub==1) system.scheduleServiceInteractive();
                    else if(sub==2) system.viewNextService();
                    else if(sub==3) system.completeNextService();
                    else if(sub==4) system.showAllServices();
                } while(sub != 5);
                break;
            }
            case 7: {
                int sub;
                do {
                    cout << "\n--- Undo / Redo ---\n";
                    cout << "1. Undo\n";
                    cout << "2. Redo\n";
                    cout << "3. Back\n";
                    sub = getValidInt("Option: ", 1, 3);
                    if(sub==1) system.undo();
                    else if(sub==2) system.redo();
                } while(sub != 3);
                break;
            }
            case 8: {
                cout << "\n--- City Connectivity ---\n";
                string start = getValidString("Enter Start Location: ");
                cout << "1. BFS (Breadth-First)\n2. DFS (Depth-First)\n";
                int m = getValidInt("Select Mode: ", 1, 2);
                if(m==1) system.bfsTraversal(start);
                else system.dfsTraversal(start);
                pressEnterToContinue();
                break;
            }
            case 9: {
                int sub;
                do {
                    cout << "\n--- Data Persistence ---\n";
                    cout << "1. Save Data\n";
                    cout << "2. Load Data\n";
                    cout << "3. Back\n";
                    sub = getValidInt("Option: ", 1, 3);
                    if(sub==1) system.save();
                    else if(sub==2) system.load();
                } while(sub != 3);
                break;
            }
            case 10:
                system.exitSystem();
                break;
        }
    } while(choice != 10);

    return 0;
}
