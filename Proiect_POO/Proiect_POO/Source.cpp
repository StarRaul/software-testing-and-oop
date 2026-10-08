#include "Source.h"

DoublyLinkedList<Worker*> Employee;

void SaveToFile() {
    ofstream out(DATA_FILE);
    if (!out) {
        cout << "Error opening file for writing!" << endl;
        return;
        }
    for (auto it = Employee.begin(); it != Employee.end(); ++it) {
        (*it)->saveToFile(out);
        }
    out.close();
    }

Worker* findWorkerById(int id) {
    for (auto it = Employee.begin(); it != Employee.end(); ++it) {
        if ((*it)->getId() == id) {
            return *it;
            }
        }
    return nullptr;
    }

bool isValidDate(const char* date) {
    int d, m, y;
    if (sscanf(date, "%d/%d/%d", &d, &m, &y) != 3) return false;

    if (y < 2024 || y > 2100) return false;
    if (m < 1 || m > 12) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) {
        daysInMonth[2] = 29;
        }

    if (d < 1 || d > daysInMonth[m]) return false;
    return true;
    }

void displayCompanyByTier() {
    cout << "\n============== COMPANY TIER LIST =============\n";
    if (Employee.isEmpty()) {
        cout << "Company is empty. You need to spawn some employees first!\n";
        return;
        }

    const char* tiers[] = { "CEO (Top Bosses)", "Department Leaders", "Team Leaders", "Workers" };

    for (int i = 0; i < 4; i++) {
        bool hasEmployeesInTier = false;
        for (auto it = Employee.begin(); it != Employee.end(); ++it) {
            bool isMatch = false;

            if (i == 0 && dynamic_cast<CEO*>(*it)) isMatch = true;
            else if (i == 1 && dynamic_cast<DepartmentLeader*>(*it)) isMatch = true;
            else if (i == 2 && dynamic_cast<TeamLeader*>(*it)) isMatch = true;
            else if (i == 3 && !dynamic_cast<Manager*>(*it)) isMatch = true;

            if (isMatch) {
                if (!hasEmployeesInTier) {
                    cout << "\n--- " << tiers[i] << " ---\n";
                    hasEmployeesInTier = true;
                    }
                (*it)->display();
                cout << "----------------------------------------------\n";
                }
            }
        }
    }

int main() {
    ifstream testFile(DATA_FILE);
    bool fileExists = testFile.good();
    testFile.close();

    if (!fileExists) {
        ofstream create(DATA_FILE);
        create.close();
        }

    int choice = -1;

    while (choice != 0) {
        cout << "\n================ MAIN MENU =================\n";
        cout << "1. Add CEO\n";
        cout << "2. Add Department Leader\n";
        cout << "3. Add Team Leader\n";
        cout << "4. Add Worker\n";
        cout << "5. Build Relationship (Assign Subordinate via ID)\n";
        cout << "6. Assign Task to Employee (via ID)\n";
        cout << "7. View Company Structure\n";
        cout << "0. Exit\n";
        cout << "Alegerea ta: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[-] Bro, folosește doar numere valide!\n\n";
            continue;
            }
        cin.ignore();

        if (choice == 1) {
            char name[100]; double salary;
            cout << "Name: "; cin.getline(name, 100);
            cout << "Salary (euro): "; cin >> salary; cin.ignore();

            Employee.add(new CEO(name, salary));
            SaveToFile();
            cout << "[+] CEO added to the roster.\n\n";
            } else if (choice == 2 || choice == 3 || choice == 4) {
                char name[100], dept[100], func[100]; double salary;
                cout << "Name: "; cin.getline(name, 100);
                cout << "Salary (euro): "; cin >> salary; cin.ignore();
                cout << "Department: "; cin.getline(dept, 100);
                cout << "Function: "; cin.getline(func, 100);

                if (choice == 2) Employee.add(new DepartmentLeader(name, salary, dept, func));
                else if (choice == 3) Employee.add(new TeamLeader(name, salary, dept, func));
                else if (choice == 4) Employee.add(new Worker(name, salary, dept, func));

                SaveToFile();
                cout << "[+] Employee added successfully.\n\n";
                } else if (choice == 5) {
                    int mgrId, subId;
                    cout << "Manager ID: "; cin >> mgrId; cin.ignore();
                    cout << "Subordinate ID: "; cin >> subId; cin.ignore();

                    Worker* wMgr = findWorkerById(mgrId);
                    Worker* wSub = findWorkerById(subId);

                    if (wMgr && wSub) {
                        Manager* mgr = dynamic_cast<Manager*>(wMgr);
                        if (mgr) {
                            mgr->addSubordinate(wSub);
                            cout << "[+] Relationship established.\n\n";
                            } else {
                            cout << "[-] Error: Employee ID " << mgrId << " is not a Manager!\n\n";
                            }
                        } else {
                        cout << "[-] Error: Check your IDs again, somebody is missing.\n\n";
                        }
                    } else if (choice == 6) {
                        int empId;
                        char desc[200], deadline[50] = "";
                        int taskType; double budget = 0.0;

                        cout << "Employee ID: "; cin >> empId; cin.ignore();
                        Worker* w = findWorkerById(empId);

                        if (!w) {
                            cout << "[-] Error: Employee not found! Press 7 to check IDs.\n\n";
                            } else {
                            cout << "Task Description: "; cin.getline(desc, 200);
                            cout << "Task Type (0 = Deadline, 1 = Budget, 2 = Both): "; cin >> taskType; cin.ignore();

                            if (taskType == 0 || taskType == 2) {
                                do {
                                    cout << "Deadline (dd/mm/yyyy): ";
                                    cin.getline(deadline, 50);
                                    if (!isValidDate(deadline)) {
                                        cout << "  [-] Invalid date format or logic. Try again.\n";
                                        }
                                    } while (!isValidDate(deadline));
                                }

                            if (taskType == 1 || taskType == 2) {
                                cout << "Budget (euro): "; cin >> budget; cin.ignore();
                                }

                            if (taskType == 0) {
                                w->addTask(new Task(desc, Task::withDeadline, deadline, 0));
                                } else if (taskType == 1) {
                                    w->addTask(new Task(desc, Task::withBudget, "", budget));
                                    } else if (taskType == 2) {
                                        w->addTask(new Task(desc, Task::withBoth, deadline, budget));
                                        } else {
                                        cout << "[-] Invalid task type.\n";
                                        }
                                    cout << "[+] Task assigned to " << w->getName() << ".\n\n";
                            }
                        } else if (choice == 7) {
                            displayCompanyByTier();
                            cout << "\n";
                            } else if (choice != 0) {
                                cout << "[-] Invalid choice. Try again.\n\n";
                                }
        }

    cout << "Cleaning up memory... See ya!\n";
    for (auto it = Employee.begin(); it != Employee.end(); ++it) {
        delete* it;
        }

    return 0;
    }