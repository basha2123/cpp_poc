#include "EmployeeService.h"

#include <iostream>
#include <limits>

namespace {
void printEmployee(const Employee& e) {
    std::cout << e.id << " | " << e.name << " | " << e.age << " | "
              << e.department << " | " << e.designation << " | Total: "
              << e.totalSalary() << '\n';
}

Employee readEmployee() {
    Employee e;
    std::cout << "Employee ID: "; std::cin >> e.id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Name: "; std::getline(std::cin, e.name);
    std::cout << "Age: "; std::cin >> e.age;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Department: "; std::getline(std::cin, e.department);
    std::cout << "Designation: "; std::getline(std::cin, e.designation);
    std::cout << "Basic salary: "; std::cin >> e.basicSalary;
    std::cout << "HRA: "; std::cin >> e.hra;
    std::cout << "Allowance: "; std::cin >> e.allowance;
    return e;
}
}

int main() {
    try {
        EmployeeRepository repository("employee.db");
        EmployeeService service(repository);
        int choice = 0;

        do {
            std::cout << "\n=== EMPLOYEE MANAGEMENT SYSTEM ===\n"
                      << "1. Add employee\n2. Display all\n3. Search\n"
                      << "4. Update\n5. Delete\n6. Exit\nChoice: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Enter a number.\n";
                continue;
            }

            if (choice == 1) {
                Employee e = readEmployee();
                std::string error;
                if (service.addEmployee(e, error)) std::cout << "Employee added.\n";
                else std::cout << "Failed: " << error << '\n';
            } else if (choice == 2) {
                for (const auto& e : service.getAllEmployees()) printEmployee(e);
            } else if (choice == 3) {
                int id; std::cout << "Employee ID: "; std::cin >> id;
                const auto e = service.getEmployee(id);
                if (e) printEmployee(*e);
                else std::cout << "Employee not found.\n";
            } else if (choice == 4) {
                Employee e = readEmployee();
                std::string error;
                if (service.updateEmployee(e, error)) std::cout << "Employee updated.\n";
                else std::cout << "Failed: " << error << '\n';
            } else if (choice == 5) {
                int id; std::cout << "Employee ID: "; std::cin >> id;
                std::cout << (service.deleteEmployee(id) ? "Employee deleted.\n" :
                                                               "Employee not found.\n");
            } else if (choice == 6) {
                std::cout << "Exiting.\n";
            } else {
                std::cout << "Invalid menu option.\n";
            }
        } while (choice != 6);
    } catch (const std::exception& ex) {
        std::cerr << "Application error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
