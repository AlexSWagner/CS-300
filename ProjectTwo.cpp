#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// Course class to hold course information
class Course {
public:
    std::string courseNumber;
    std::string courseTitle;
    std::vector<std::string> prerequisites;

    // Default constructor
    Course() = default;

    // Parameterized constructor
    Course(std::string courseNumber, std::string courseTitle, std::vector<std::string> prerequisites)
        : courseNumber(courseNumber), courseTitle(courseTitle), prerequisites(prerequisites) {}
};

// Function to load course data from CSV file
void loadCourses(const std::string& filename, std::unordered_map<std::string, Course>& courses) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file\n";
        return;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string courseNumber, courseTitle, prerequisite;
        std::vector<std::string> prerequisites;

        std::getline(ss, courseNumber, ',');
        std::getline(ss, courseTitle, ',');

        while (std::getline(ss, prerequisite, ',')) {
            prerequisites.push_back(prerequisite);
        }

        Course course(courseNumber, courseTitle, prerequisites);
        courses[courseNumber] = course;
    }

    file.close();
}

// Function to validate courses and prerequisites
void validateCourses(const std::unordered_map<std::string, Course>& courses) {
    std::unordered_set<std::string> courseNumbers;
    for (const auto& coursePair : courses) {
        courseNumbers.insert(coursePair.first);
    }

    for (const auto& coursePair : courses) {
        for (const auto& prerequisite : coursePair.second.prerequisites) {
            if (courseNumbers.find(prerequisite) == courseNumbers.end()) {
                std::cerr << "Error: Prerequisite " << prerequisite << " for course " << coursePair.first << " does not exist\n";
            }
        }
    }
}

// Function to print the course list
void printCourses(const std::unordered_map<std::string, Course>& courses) {
    std::vector<std::string> courseNumbers;
    for (const auto& coursePair : courses) {
        courseNumbers.push_back(coursePair.first);
    }
    std::sort(courseNumbers.begin(), courseNumbers.end());

    std::cout << "Here is a sample schedule:\n";
    for (const auto& courseNumber : courseNumbers) {
        const auto& course = courses.at(courseNumber);
        std::cout << course.courseNumber << ", " << course.courseTitle << "\n";
    }
}

// Function to print course information
void printCourseInfo(const std::unordered_map<std::string, Course>& courses) {
    std::string courseNumber;
    std::cout << "What course do you want to know about? ";
    std::cin >> courseNumber;

    // Convert input to uppercase to handle case-insensitivity
    std::transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    if (courses.find(courseNumber) != courses.end()) {
        const auto& course = courses.at(courseNumber);
        std::cout << course.courseNumber << ", " << course.courseTitle << "\n";
        if (!course.prerequisites.empty()) {
            std::cout << "Prerequisites: ";
            for (const auto& prereq : course.prerequisites) {
                std::cout << prereq << (prereq == course.prerequisites.back() ? "" : ", ");
            }
            std::cout << "\n";
        }
        else {
            std::cout << "Prerequisites: None\n";
        }
    }
    else {
        std::cout << "Course not found.\n";
    }
}

// Function to print the menu
void printMenu() {
    std::cout << "\n1. Load Data Structure.\n";
    std::cout << "2. Print Course List.\n";
    std::cout << "3. Print Course.\n";
    std::cout << "9. Exit.\n";
}

// Main function
int main() {
    std::cout << "Welcome to the course planner.\n";
    std::unordered_map<std::string, Course> courses;
    int choice = 0;

    while (choice != 9) {
        printMenu();
        std::cout << "What would you like to do? ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string filename;
            std::cout << "Enter the filename (with full path if not in the same directory): ";
            std::cin >> filename;
            loadCourses(filename, courses);
            validateCourses(courses);
            if (!courses.empty()) {
                std::cout << "Data loaded successfully.\n";
            }
            else {
                std::cout << "No data loaded. Please check the file and try again.\n";
            }
            break;
        }
        case 2:
            if (courses.empty()) {
                std::cout << "No data loaded. Please load the data first.\n";
            }
            else {
                printCourses(courses);
            }
            break;
        case 3:
            if (courses.empty()) {
                std::cout << "No data loaded. Please load the data first.\n";
            }
            else {
                printCourseInfo(courses);
            }
            break;
        case 9:
            std::cout << "Thank you for using the course planner!\n";
            break;
        default:
            std::cout << choice << " is not a valid option.\n";
        }
    }

    return 0;
}
