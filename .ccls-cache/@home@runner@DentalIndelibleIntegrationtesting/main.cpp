#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <map>
#include <chrono>
#include <thread>

using namespace std;

class Task {
public:
    string description;
    bool completed;
    chrono::system_clock::time_point deadline;

    Task(const string& desc, const chrono::system_clock::time_point& dl)
        : description(desc), completed(false), deadline(dl) {}
};

class Project {
public:
    string name;
    string description;
    string type;
    vector<Task> tasks;
    chrono::system_clock::time_point creationDate;
    chrono::system_clock::time_point deadline;

    Project(const string& n, const string& d, const string& t)
        : name(n), description(d), type(t) {
        creationDate = chrono::system_clock::now();
    }

    void addTask(const string& description, const chrono::system_clock::time_point& deadline) {
        tasks.emplace_back(description, deadline);
    }

    int getCompletedTaskCount() const {
        return count_if(tasks.begin(), tasks.end(), [](const Task& t) { return t.completed; });
    }

    double getProgress() const {
        if (tasks.empty()) return 0.0;
        return (double)getCompletedTaskCount() / tasks.size() * 100.0;
    }
};

class WebDevTerminal {
private:
    vector<Project> projects;
    string username;
    map<string, string> notes;

    void clearScreen() {
        cout << string(50, '\n');
    }

    void displayHeader(const string& currentPage) {
        cout << "========================================\n";
        cout << "Web Development Terminal - " << currentPage << "\n";
        cout << "========================================\n";
    }

    void displayFooter() {
        cout << "----------------------------------------\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    string getFormattedDate(const chrono::system_clock::time_point& tp) {
        time_t t = chrono::system_clock::to_time_t(tp);
        string ts = ctime(&t);
        ts.resize(ts.size() - 1);
        return ts;
    }

public:
    WebDevTerminal(const string& user) : username(user) {}

    void displayMainMenu() {
        clearScreen();
        displayHeader("Main Menu");
        cout << "1. Create a new project\n";
        cout << "2. List all projects\n";
        cout << "3. Manage project tasks\n";
        cout << "4. Search projects\n";
        cout << "5. Add/View notes\n";
        cout << "6. Generate project report\n";
        cout << "7. Set project deadline\n";
        cout << "8. Save projects to file\n";
        cout << "9. Load projects from file\n";
        cout << "10. Exit\n";
        cout << "Choose an option: ";
    }

    void createProject() {
        clearScreen();
        displayHeader("Create Project");
        string name, description, type;

        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter project description: ";
        getline(cin, description);
        cout << "Enter project type: ";
        getline(cin, type);

        projects.emplace_back(name, description, type);
        cout << "Project '" << name << "' created successfully!\n";
        displayFooter();
    }

    void listProjects() {
        clearScreen();
        displayHeader("Projects List");
        if (projects.empty()) {
            cout << "No projects found.\n";
        } else {
            cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Type" << setw(20) << "Creation Date" << "Progress\n";
            cout << string(80, '-') << "\n";
            for (size_t i = 0; i < projects.size(); ++i) {
                cout << left << setw(5) << i + 1 
                     << setw(20) << projects[i].name 
                     << setw(15) << projects[i].type
                     << setw(20) << getFormattedDate(projects[i].creationDate)
                     << fixed << setprecision(2) << projects[i].getProgress() << "%\n";
            }
        }
        displayFooter();
    }

    void manageProjectTasks() {
        clearScreen();
        displayHeader("Manage Project Tasks");
        if (projects.empty()) {
            cout << "No projects available.\n";
            displayFooter();
            return;
        }

        cout << "Select a project:\n";
        for (size_t i = 0; i < projects.size(); ++i) {
            cout << i + 1 << ". " << projects[i].name << "\n";
        }

        size_t projectIndex;
        cout << "Enter project number: ";
        cin >> projectIndex;
        projectIndex--;

        if (projectIndex >= projects.size()) {
            cout << "Invalid project number.\n";
            displayFooter();
            return;
        }

        Project& project = projects[projectIndex];

        while (true) {
            clearScreen();
            displayHeader("Manage Tasks for " + project.name);
            cout << "1. Add task\n";
            cout << "2. List tasks\n";
            cout << "3. Mark task as completed\n";
            cout << "4. Return to main menu\n";
            cout << "Choose an option: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1: {
                    string taskDesc;
                    cout << "Enter task description: ";
                    cin.ignore();
                    getline(cin, taskDesc);

                    int days;
                    cout << "Enter task deadline (days from now): ";
                    cin >> days;

                    auto deadline = chrono::system_clock::now() + chrono::hours(24 * days);
                    project.addTask(taskDesc, deadline);
                    cout << "Task added successfully!\n";
                    break;
                }
                case 2: {
                    cout << "Tasks for " << project.name << ":\n";
                    for (size_t i = 0; i < project.tasks.size(); ++i) {
                        cout << i + 1 << ". " << (project.tasks[i].completed ? "[X] " : "[ ] ")
                             << project.tasks[i].description << " (Due: " 
                             << getFormattedDate(project.tasks[i].deadline) << ")\n";
                    }
                    break;
                }
                case 3: {
                    if (project.tasks.empty()) {
                        cout << "No tasks available.\n";
                    } else {
                        cout << "Enter task number to mark as completed: ";
                        size_t taskIndex;
                        cin >> taskIndex;
                        taskIndex--;
                        if (taskIndex < project.tasks.size()) {
                            project.tasks[taskIndex].completed = true;
                            cout << "Task marked as completed.\n";
                        } else {
                            cout << "Invalid task number.\n";
                        }
                    }
                    break;
                }
                case 4:
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
            displayFooter();
        }
    }

    void searchProjects() {
        clearScreen();
        displayHeader("Search Projects");
        string searchTerm;
        cout << "Enter search term: ";
        cin.ignore();
        getline(cin, searchTerm);

        vector<Project> results;
        for (const auto& project : projects) {
            if (project.name.find(searchTerm) != string::npos || 
                project.description.find(searchTerm) != string::npos ||
                project.type.find(searchTerm) != string::npos) {
                results.push_back(project);
            }
        }

        if (results.empty()) {
            cout << "No matching projects found.\n";
        } else {
            cout << "Matching Projects:\n";
            cout << left << setw(20) << "Name" << setw(15) << "Type" << "Description\n";
            cout << string(80, '-') << "\n";
            for (const auto& project : results) {
                cout << left << setw(20) << project.name 
                     << setw(15) << project.type
                     << project.description << "\n";
            }
        }
        displayFooter();
    }

    void manageNotes() {
        clearScreen();
        displayHeader("Manage Notes");
        cout << "1. Add note\n";
        cout << "2. View notes\n";
        cout << "3. Return to main menu\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string noteTitle, noteContent;
                cout << "Enter note title: ";
                cin.ignore();
                getline(cin, noteTitle);
                cout << "Enter note content: ";
                getline(cin, noteContent);
                notes[noteTitle] = noteContent;
                cout << "Note added successfully!\n";
                break;
            }
            case 2: {
                if (notes.empty()) {
                    cout << "No notes available.\n";
                } else {
                    for (const auto& note : notes) {
                        cout << "Title: " << note.first << "\n";
                        cout << "Content: " << note.second << "\n\n";
                    }
                }
                break;
            }
            case 3:
                return;
            default:
                cout << "Invalid choice.\n";
        }
        displayFooter();
    }

    void generateProjectReport() {
        clearScreen();
        displayHeader("Generate Project Report");
        if (projects.empty()) {
            cout << "No projects available.\n";
            displayFooter();
            return;
        }

        cout << "Select a project:\n";
        for (size_t i = 0; i < projects.size(); ++i) {
            cout << i + 1 << ". " << projects[i].name << "\n";
        }

        size_t projectIndex;
        cout << "Enter project number: ";
        cin >> projectIndex;
        projectIndex--;

        if (projectIndex >= projects.size()) {
            cout << "Invalid project number.\n";
            displayFooter();
            return;
        }

        const Project& project = projects[projectIndex];

        cout << "\nProject Report for " << project.name << "\n";
        cout << "==============================\n";
        cout << "Description: " << project.description << "\n";
        cout << "Type: " << project.type << "\n";
        cout << "Creation Date: " << getFormattedDate(project.creationDate) << "\n";
        if (project.deadline != chrono::system_clock::time_point()) {
            cout << "Deadline: " << getFormattedDate(project.deadline) << "\n";
        }
        cout << "Progress: " << fixed << setprecision(2) << project.getProgress() << "%\n";
        cout << "Tasks: " << project.tasks.size() << " (Completed: " << project.getCompletedTaskCount() << ")\n\n";

        cout << "Task List:\n";
        for (size_t i = 0; i < project.tasks.size(); ++i) {
            cout << i + 1 << ". " << (project.tasks[i].completed ? "[X] " : "[ ] ")
                 << project.tasks[i].description << " (Due: " 
                 << getFormattedDate(project.tasks[i].deadline) << ")\n";
        }

        displayFooter();
    }

    void setProjectDeadline() {
        clearScreen();
        displayHeader("Set Project Deadline");
        if (projects.empty()) {
            cout << "No projects available.\n";
            displayFooter();
            return;
        }

        cout << "Select a project:\n";
        for (size_t i = 0; i < projects.size(); ++i) {
            cout << i + 1 << ". " << projects[i].name << "\n";
        }

        size_t projectIndex;
        cout << "Enter project number: ";
        cin >> projectIndex;
        projectIndex--;

        if (projectIndex >= projects.size()) {
            cout << "Invalid project number.\n";
            displayFooter();
            return;
        }

        Project& project = projects[projectIndex];

        int days;
        cout << "Enter project deadline (days from now): ";
        cin >> days;

        project.deadline = chrono::system_clock::now() + chrono::hours(24 * days);
        cout << "Deadline set successfully for " << project.name << ".\n";
        displayFooter();
    }

    void saveProjectsToFile() {
        ofstream file("projects.txt");
        if (file.is_open()) {
            for (const auto& project : projects) {
                file << project.name << "|" << project.description << "|" << project.type << "|"
                     << chrono::system_clock::to_time_t(project.creationDate) << "|"
                     << chrono::system_clock::to_time_t(project.deadline) << "\n";

                for (const auto& task : project.tasks) {
                    file << "TASK|" << task.description << "|" << task.completed << "|"
                         << chrono::system_clock::to_time_t(task.deadline) << "\n";
                }
                file << "ENDPROJECT\n";
            }
            file.close();
            cout << "Projects saved to 'projects.txt'.\n";
        } else {
            cout << "Error: Unable to open file for saving.\n";
        }
        displayFooter();
    }

    void loadProjectsFromFile() {
        ifstream file("projects.txt");
        if (file.is_open()) {
            projects.clear();
            string line;
            Project* currentProject = nullptr;

            while (getline(file, line)) {
                if (line == "ENDPROJECT") {
                    currentProject = nullptr;
                    continue;
                }

                size_t pos = line.find("|");
                if (pos != string::npos) {
                    string type = line.substr(0, pos);
                    if (type == "TASK" && currentProject != nullptr) {
                        line = line.substr(pos + 1);
                        pos = line.find("|");
                        string description = line.substr(0, pos);
                        line = line.substr(pos + 1);
                        pos = line.find("|");
                        bool completed = (line.substr(0, pos) == "1");
                        line = line.substr(pos + 1);
                        time_t deadline = stoll(line);
                        currentProject->addTask(description, chrono::system_clock::from_time_t(deadline));
                        currentProject->tasks.back().completed = completed;
                    } else {
                        string name = line.substr(0, pos);
                        line = line.substr(pos + 1);
                        pos = line.find("|");
                        string description = line.substr(0, pos);
                        line = 

 line.substr(pos + 1);
                        pos = line.find("|");
                        string projectType = line.substr(0, pos);
                        line = line.substr(pos + 1);
                        pos = line.find("|");
                        time_t creationDate = stoll(line.substr(0, pos));
                        time_t deadline = stoll(line.substr(pos + 1));

                        projects.emplace_back(name, description, projectType);
                        currentProject = &projects.back();
                        currentProject->creationDate = chrono::system_clock::from_time_t(creationDate);
                        currentProject->deadline = chrono::system_clock::from_time_t(deadline);
                    }
                }
            }
            file.close();
            cout << "Projects loaded from 'projects.txt'.\n";
        } else {
            cout << "Error: Unable to open file for loading.\n";
        }
        displayFooter();
    }

    void run() {
        int choice;
        do {
            displayMainMenu();
            cin >> choice;

            switch (choice) {
                case 1: createProject(); break;
                case 2: listProjects(); break;
                case 3: manageProjectTasks(); break;
                case 4: searchProjects(); break;
                case 5: manageNotes(); break;
                case 6: generateProjectReport(); break;
                case 7: setProjectDeadline(); break;
                case 8: saveProjectsToFile(); break;
                case 9: loadProjectsFromFile(); break;
                case 10: 
                    cout << "Exiting program. Goodbye, " << username << "!\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    displayFooter();
                    break;
            }

            if (choice != 10) {
                clearScreen();
            }
        } while (choice != 10);
    }
};

int main() {
    string user;
    cout << "Enter your username: ";
    cin >> user;

    WebDevTerminal terminal(user);
    terminal.run();

    return 0;
}