#include "EmployeeService.h"

EmployeeService::EmployeeService(EmployeeRepository& repository)
    : repository_(repository) {}

bool EmployeeService::isValid(const Employee& e, std::string& error) const {
    if (e.id <= 0) error = "Employee ID must be positive";
    else if (e.name.empty()) error = "Employee name is required";
    else if (e.age < 18 || e.age > 65) error = "Age must be between 18 and 65";
    else if (e.department.empty()) error = "Department is required";
    else if (e.designation.empty()) error = "Designation is required";
    else if (e.basicSalary < 0 || e.hra < 0 || e.allowance < 0)
        error = "Salary components cannot be negative";
    else {
        error.clear();
        return true;
    }
    return false;
}

bool EmployeeService::addEmployee(const Employee& e, std::string& error) {
    if (!isValid(e, error)) return false;
    if (!repository_.add(e)) {
        error = "Employee ID already exists";
        return false;
    }
    return true;
}

std::optional<Employee> EmployeeService::getEmployee(int id) const {
    if (id <= 0) return std::nullopt;
    return repository_.findById(id);
}

std::vector<Employee> EmployeeService::getAllEmployees() const {
    return repository_.getAll();
}

bool EmployeeService::updateEmployee(const Employee& e, std::string& error) {
    if (!isValid(e, error)) return false;
    if (!repository_.update(e)) {
        error = "Employee not found";
        return false;
    }
    return true;
}

bool EmployeeService::deleteEmployee(int id) {
    if (id <= 0) return false;
    return repository_.remove(id);
}
