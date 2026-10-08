#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

const char* DATA_FILE = "company_data.txt";
const int MAX_LINE_LENGTH = 1024;

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& data, Node* prev = nullptr, Node* next = nullptr) :
            data(data), prev(prev), next(next) {
            }
        };
    Node* head;
    Node* tail;
    size_t size;

public:
    DoublyLinkedList() :head(nullptr), tail(nullptr), size(0) {}
    ~DoublyLinkedList() { clear(); }

    void add(const T& data) {
        Node* newNode = new Node(data, tail, nullptr);
        if (tail) { tail->next = newNode; } else { head = newNode; }
        tail = newNode;
        size++;
        }

    bool remove(const T& data) {
        Node* node = head;
        while (node) {
            if (node->data == data) {
                if (node->prev) { node->prev->next = node->next; } else { head = node->next; }
                if (node->next) { node->next->prev = node->prev; } else { tail = node->prev; }

                delete node;
                size--;
                return true;
                }
            node = node->next;
            }
        return false;
        }

    void clear() {
        Node* node = head;
        while (node) {
            Node* next = node->next;
            delete node;
            node = next;
            }
        head = tail = nullptr;
        size = 0;
        }

    bool isEmpty() const { return size == 0; }
    size_t getSize() const { return size; }

    class Iterator {
        Node* node;
    public:
        Iterator(Node* node) : node(node) {}
        Iterator& operator++() {
            node = node->next;
            return *this;
            }
        T& operator*() const { return node->data; }
        T* operator->() const { return &node->data; }
        bool operator!=(const Iterator& data) const { return node != data.node; }
        };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
    };

// ==================================================================================
class Task;
class Worker;
class Manager;
void SaveToFile();

extern DoublyLinkedList<Worker*> Employee;
// ==================================================================================

class Task {
public:
    enum TaskType { withDeadline, withBudget, withBoth };
protected:
    TaskType taskType;
    char* description;
    char* deadline;
    double budget;
public:
    Task(const char* description, TaskType type, const char* deadline = nullptr, double budget = 0.0) {
        this->description = new char[strlen(description) + 1];
        strcpy(this->description, description);
        this->taskType = type;

        if (deadline != nullptr) {
            this->deadline = new char[strlen(deadline) + 1];
            strcpy(this->deadline, deadline);
            } else {
            this->deadline = nullptr;
            }

        this->budget = budget;
        if (type == withDeadline) { this->budget = 0.0; } else if (type == withBudget) {
            if (this->deadline) { delete[] this->deadline; this->deadline = nullptr; }
            }
        }

    virtual ~Task() {
        delete[] description;
        if (deadline) delete[] deadline;
        }

    const char* getDescription() const { return description; }
    TaskType getTaskType() const { return taskType; }

    virtual void display() const {
        cout << "    - Task: " << description << " | Type: " << (taskType == withDeadline ? "With Deadline" : taskType == withBudget ? "With Budget" : "With Both");
        if (deadline != nullptr) { cout << " | Deadline: " << deadline; }
        if (budget > 0.0) { cout << " | Budget: " << budget << " euro"; }
        cout << endl;
        }

    virtual void saveToFile(ofstream& out) const {
        out << "Task:" << description << " | Type: " << (taskType == withDeadline ? "With Deadline" : taskType == withBudget ? "With Budget" : "With Both");
        if (deadline) { out << " | Deadline: " << deadline; }
        if (budget > 0.0) { out << " | Budget: " << budget << " euro"; }
        out << endl;
        }
    };

// ==================================================================================

class Worker {
protected:
    int id;
    char* name;
    double salary;
    char* department;
    char* function;
    DoublyLinkedList<Task*>* tasks;

public:
    Worker(const char* name, double salary, const char* department, const char* function) :
        salary(salary), tasks(new DoublyLinkedList<Task*>()) {

        static int globalId = 1;
        this->id = globalId++;

        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->department = new char[strlen(department) + 1];
        strcpy(this->department, department);
        this->function = new char[strlen(function) + 1];
        strcpy(this->function, function);
        }

    virtual ~Worker() {
        delete[] name;
        delete[] department;
        delete[] function;
        delete tasks;
        }

    int getId() const { return id; }
    const char* getName() const { return name; }
    double getSalary() const { return salary; }
    const char* getDepartment() const { return department; }
    const char* getFunction() const { return function; }

    void setName(const char* newName) {
        delete[] name;
        name = new char[strlen(newName) + 1];
        strcpy(name, newName);
        SaveToFile();
        }

    void setSalary(double newSalary) {
        salary = newSalary;
        SaveToFile();
        }

    void setDepartment(const char* newDepartment) {
        delete[] department;
        department = new char[strlen(newDepartment) + 1];
        strcpy(department, newDepartment);
        SaveToFile();
        }

    void setFunction(const char* newFunction) {
        delete[] function;
        function = new char[strlen(newFunction) + 1];
        strcpy(function, newFunction);
        SaveToFile();
        }

    void addTask(Task* task) {
        tasks->add(task);
        SaveToFile();
        }

    bool removeTask(const char* taskDescription) {
        for (auto it = tasks->begin(); it != tasks->end(); ++it) {
            if (strcmp((*it)->getDescription(), taskDescription) == 0) {
                Task* toRemove = *it;
                tasks->remove(toRemove);
                delete toRemove;
                SaveToFile();
                return true;
                }
            }
        return false;
        }

    virtual void display() const {
        cout << "Employee [ID: " << id << "] - " << name << endl;
        cout << "  Department: " << department << endl;
        cout << "  Salary: " << salary << " euro" << endl;
        cout << "  Function: " << function << endl;

        if (!tasks->isEmpty()) {
            cout << "  Tasks:\n";
            for (auto it = tasks->begin(); it != tasks->end(); ++it) {
                (*it)->display();
                }
            }
        }

    virtual void saveToFile(ofstream& out) const {
        out << "Worker:" << name << "|" << salary << "|" << department << "|" << function << endl;
        for (auto it = tasks->begin(); it != tasks->end(); ++it) {
            out << "WorkerTask:" << name << "|";
            (*it)->saveToFile(out);
            }
        }
    };

// ==================================================================================

class Manager : public Worker {
protected:
    DoublyLinkedList<Worker*>* subordinates;

public:
    Manager(const char* name, double salary, const char* department, const char* function) :
        Worker(name, salary, department, function), subordinates(new DoublyLinkedList<Worker*>()) {
        }

    virtual ~Manager() { delete subordinates; }

    void addSubordinate(Worker* worker) {
        subordinates->add(worker);
        SaveToFile();
        }

    bool removeSubordinate(const char* workerName) {
        for (auto it = subordinates->begin(); it != subordinates->end(); ++it) {
            if (strcmp((*it)->getName(), workerName) == 0) {
                subordinates->remove(*it);
                SaveToFile();
                return true;
                }
            }
        return false;
        }

    virtual const char* getManagerType() const = 0;

    void display() const override {
        Worker::display();
        cout << "  Manager Type: " << getManagerType() << endl;
        if (!subordinates->isEmpty()) {
            cout << "  Subordinates:\n";
            for (auto it = subordinates->begin(); it != subordinates->end(); ++it) {
                cout << "    - " << (*it)->getName() << " [ID: " << (*it)->getId() << "]" << endl;
                }
            }
        }

    void saveToFile(ofstream& out) const override {
        out << getType() << ":" << name << "|" << salary << "|" << department << "|" << function << endl;

        for (auto it = subordinates->begin(); it != subordinates->end(); ++it) {
            out << "Subordinate:" << name << "|" << (*it)->getName() << endl;
            }
        for (auto it = tasks->begin(); it != tasks->end(); ++it) {
            out << "ManagerTask:" << name << "|";
            (*it)->saveToFile(out);
            }
        }

    virtual const char* getType() const = 0;
    };

// ==================================================================================

class CEO : public Manager {
public:
    CEO(const char* name, double salary)
        : Manager(name, salary, "Executive", "CEO") {
        }
    const char* getManagerType() const override { return "CEO"; }
    const char* getType() const override { return "CEO"; }
    };

class DepartmentLeader : public Manager {
public:
    DepartmentLeader(const char* name, double salary, const char* department, const char* function) :
        Manager(name, salary, department, function) {
        }
    const char* getManagerType() const override { return "Department Leader"; }
    const char* getType() const override { return "DepartmentLeader"; }
    };

class TeamLeader : public Manager {
public:
    TeamLeader(const char* name, double salary, const char* department, const char* function)
        : Manager(name, salary, department, function) {
        }
    const char* getManagerType() const override { return "Team Leader"; }
    const char* getType() const override { return "TeamLeader"; }
    };