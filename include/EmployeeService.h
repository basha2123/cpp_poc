#pragma once

#include "EmployeeRepository.h"
#include <optional>
#include <string>
#include <vector>

class EmployeeService {
public:
    explicit EmployeeService(EmployeeRepository& repository);

    bool addEmployee(const Employee& employee, std::string& error);
    std::optional<Employee> getEmployee(int id) const;
    std::vector<Employee> getAllEmployees() const;
    bool updateEmployee(const Employee& employee, std::string& error);
    bool deleteEmployee(int id);

private:
    EmployeeRepository& repository_;
    bool isValid(const Employee& employee, std::string& error) const;
};
