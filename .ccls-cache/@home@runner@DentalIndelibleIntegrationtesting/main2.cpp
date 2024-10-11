#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <ctime>

class Task {
public:
    std::string description;
    bool completed;
    int priority;
    time_t due_date;

    Task(const std::string& desc, int prio, time_t due) 
        : description(desc), completed(false), priority(prio), due_date(due) {}
};

class Project {
public:
    std::string name;
    std::vector<Task> tasks;

    Project(const std::string& n) : name(n) {}

    void addTask(const std::string& desc, int priority, time_t due_date) {
        tasks.emplace_back(desc, priority, due_date);
    }

    float getProgress() const {
        if (tasks.empty()) return 0.0f;
        int completed = std::count_if(tasks.begin(), tasks.end(), [](const Task& t) { return t.completed; });
        return (float)completed / tasks.size() * 100.0f;
    }
};

class ProjectManager {
private:
    std::vector<Project> projects;

    void clearScreen() {
        std::cout << std::string(50, '\n');
    }

    void displayHeader(const std::string& title) {
        std::cout << "\n=== " << title << " ===\n\n";
    }

    void waitForEnter() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    int getMenuChoice(int max_choice) {
        int choice;
        std::cout << "\nEnter your choice (1-" << max_choice << "): ";
        while (!(std::cin >> choice) || choice < 1 || choice > max_choice) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }

    std::string getInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }

    time_t getDate() {
        int year, month, day;
        std::cout << "Enter due date (YYYY MM DD): ";
        std::cin >> year >> month >> day;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        struct tm timeinfo = {0};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;

        return mktime(&timeinfo);
    }

    void displayProjects() {
        if (projects.empty()) {
            std::cout << "No projects available.\n";
            return;
        }

        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(20) << "Project Name" 
                  << std::setw(10) << "Progress" 
                  << "Tasks\n";
        std::cout << std::string(50, '-') << '\n';

        for (size_t i = 0; i < projects.size(); ++i) {
            std::cout << std::left << std::setw(5) << i + 1 
                      << std::setw(20) << projects[i].name 
                      << std::setw(10) << std::fixed << std::setprecision(1) << projects[i].getProgress() << "% "
                      << projects[i].tasks.size() << '\n';
        }
    }

    void createProject() {
        std::string name = getInput("Enter project name: ");
        projects.emplace_back(name);
        std::cout << "Project '" << name << "' created successfully!\n";
    }

    void manageProject() {
        displayProjects();
        if (projects.empty()) return;

        int projectId = getMenuChoice(projects.size()) - 1;
        Project& project = projects[projectId];

        while (true) {
            clearScreen();
            displayHeader("Managing Project: " + project.name);
            std::cout << "1. Add Task\n2. View Tasks\n3. Complete Task\n4. Back to Main Menu\n";

            switch (getMenuChoice(4)) {
                case 1: {
                    std::string desc = getInput("Enter task description: ");
                    int priority;
                    std::cout << "Enter priority (1-5, 1 being highest): ";
                    std::cin >> priority;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    time_t due_date = getDate();
                    project.addTask(desc, priority, due_date);
                    std::cout << "Task added successfully!\n";
                    break;
                }
                case 2: {
                    if (project.tasks.empty()) {
                        std::cout << "No tasks available.\n";
                    } else {
                        std::cout << std::left << std::setw(5) << "ID" 
                                  << std::setw(30) << "Description" 
                                  << std::setw(10) << "Priority"
                                  << std::setw(15) << "Due Date"
                                  << "Status\n";
                        std::cout << std::string(70, '-') << '\n';
                        for (size_t i = 0; i < project.tasks.size(); ++i) {
                            const Task& task = project.tasks[i];
                            std::cout << std::left << std::setw(5) << i + 1 
                                      << std::setw(30) << task.description 
                                      << std::setw(10) << task.priority
                                      << std::setw(15) << std::put_time(localtime(&task.due_date), "%Y-%m-%d")
                                      << (task.completed ? "Completed" : "Pending") << '\n';
                        }
                    }
                    break;
                }
                case 3: {
                    if (project.tasks.empty()) {
                        std::cout << "No tasks available.\n";
                    } else {
                        std::cout << "Enter task ID to mark as completed: ";
                        int taskId = getMenuChoice(project.tasks.size()) - 1;
                        project.tasks[taskId].completed = true;
                        std::cout << "Task marked as completed.\n";
                    }
                    break;
                }
                case 4:
                    return;
            }
            waitForEnter();
        }
    }

public:
    void run() {
        while (true) {
            clearScreen();
            displayHeader("Project Management System");
            std::cout << "1. Create New Project\n"
                      << "2. Manage Project\n"
                      << "3. View All Projects\n"
                      << "4. Exit\n";

            switch (getMenuChoice(4)) {
                case 1:
                    createProject();
                    break;
                case 2:
                    manageProject();
                    break;
                case 3:
                    displayProjects();
                    break;
                case 4:
                    std::cout << "Thank you for using the Project Management System. Goodbye!\n";
                    return;
            }
            waitForEnter();
        }
    }
};

int main() {
    ProjectManager manager;
    manager.run();
    return 0;
}