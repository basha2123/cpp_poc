#pragma once

#include "Employee.h"
#include <optional>
#include <string>
#include <vector>
#include <sqlite3.h>

class EmployeeRepository {
public:
    explicit EmployeeRepository(const std::string& databasePath);
    ~EmployeeRepository();

    EmployeeRepository(const EmployeeRepository&) = delete;
    EmployeeRepository& operator=(const EmployeeRepository&) = delete;

    bool add(const Employee& employee);
    std::optional<Employee> findById(int id) const;
    std::vector<Employee> getAll() const;
    bool update(const Employee& employee);
    bool remove(int id);

private:
    sqlite3* db_{nullptr};
    void createTable();
    void checkDatabase(int result, const std::string& operation) const;
};
