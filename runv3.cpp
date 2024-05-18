#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>

using namespace std;

class Task {
public:
    string title;
    string description;
    string deadline;
    int priority;
    bool completed;

    Task(string _title, string _description, string _deadline, int _priority) {
        title = _title;
        description = _description;
        deadline = _deadline;
        priority = _priority;
        completed = false;
    }
};

class TaskList {
public:
    vector<shared_ptr<Task>> tasks;

    void addTask(shared_ptr<Task> newTask) {
        tasks.push_back(newTask);
        saveToFile("tasks.txt");
    }

    void showTasks() {
        if (tasks.empty()) {
            cout << "No tasks found!" << endl;
        } else {
            sort(tasks.begin(), tasks.end(), [](const shared_ptr<Task>& a, const shared_ptr<Task>& b) {
                return a->priority > b->priority;
            });

            for (int i = 0; i < tasks.size(); ++i) {
                cout << "Task #" << i+1 << ":" << endl;
                cout << "Title: " << tasks[i]->title << endl;
                cout << "Description: " << tasks[i]->description << endl;
                cout << "Deadline: " << tasks[i]->deadline << endl;
                cout << "Priority: " << tasks[i]->priority << endl;
                cout << "Completed: " << (tasks[i]->completed ? "Yes" : "No") << endl;
                cout << endl;
            }
        }
    }

    void deleteTask(int index) {
        if (index >= 0 && index < tasks.size()) {
            tasks.erase(tasks.begin() + index);
            saveToFile("tasks.txt");
            cout << "Task deleted successfully!" << endl;
        } else {
            cout << "Invalid task index!" << endl;
        }
    }

    void editTask(int index) {
        if (index >= 0 && index < tasks.size()) {
            cout << "Enter new title: ";
            cin.ignore();
            getline(cin, tasks[index]->title);
            cout << "Enter new description: ";
            getline(cin, tasks[index]->description);
            cout << "Enter new deadline: ";
            getline(cin, tasks[index]->deadline);
            cout << "Enter new priority: ";
            cin >> tasks[index]->priority;
            saveToFile("tasks.txt");
            cout << "Task edited successfully!" << endl;
        } else {
            cout << "Invalid task index!" << endl;
        }
    }

    void markTaskCompleted(int index) {
        if (index >= 0 && index < tasks.size()) {
            tasks[index]->completed = true;
            saveToFile("tasks.txt");
            cout << "Task marked as completed!" << endl;
        } else {
            cout << "Invalid task index!" << endl;
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        for (const auto& task : tasks) {
            file << task->title << ","
                 << task->description << ","
                 << task->deadline << ","
                 << task->priority << ","
                 << task->completed << endl;
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string title, description, deadline, priorityStr, completedStr;
        tasks.clear();
        while (getline(file, title, ',') &&
               getline(file, description, ',') &&
               getline(file, deadline, ',') &&
               getline(file, priorityStr, ',') &&
               getline(file, completedStr)) {
            int priority = stoi(priorityStr);
            bool completed = (completedStr == "1");
            auto task = make_shared<Task>(title, description, deadline, priority);
            task->completed = completed;
            tasks.push_back(task);
        }
        file.close();
    }
};

class Menu {
public:
    void displayMenu() {
        cout << "............................................" << endl;
        cout << "                 Todo List" << endl;
        cout << "............................................" << endl << endl;
        cout << "::::::::::::::: PROGRAM MENU :::::::::::::::" << endl;
        cout << "1. Add New Task" << endl;
        cout << "2. Display All Tasks" << endl;
        cout << "3. Mark Task as Completed" << endl;
        cout << "4. Edit Task" << endl;
        cout << "5. Remove Task" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
    }
};

int main() {
    TaskList taskList;
    taskList.loadFromFile("tasks.txt");
    Menu menu;
    int choice;

    do {
        menu.displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string title, description, deadline;
                int priority;
                cout << "Enter task title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter task description: ";
                getline(cin, description);
                cout << "Enter task deadline: ";
                getline(cin, deadline);
                cout << "Enter task priority (higher number means higher priority): ";
                cin >> priority;
                auto newTask = make_shared<Task>(title, description, deadline, priority);
                taskList.addTask(newTask);
                break;
            }
            case 2:
                taskList.showTasks();
                break;
            case 3: {
                int index;
                cout << "Enter index of task to mark as completed: ";
                cin >> index;
                taskList.markTaskCompleted(index - 1);
                break;
            }
            case 4: {
                int index;
                cout << "Enter index of task to edit: ";
                cin >> index;
                taskList.editTask(index - 1);
                break;
            }
            case 5: {
                int index;
                cout << "Enter index of task to delete: ";
                cin >> index;
                taskList.deleteTask(index - 1);
                break;
            }
            case 6:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    } while (choice != 6);

    taskList.saveToFile("tasks.txt");

    return 0;
}
