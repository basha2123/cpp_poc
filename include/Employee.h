#pragma once

#include <string>

struct Employee {
    int id{};
    std::string name;
    int age{};
    std::string department;
    std::string designation;
    double basicSalary{};
    double hra{};
    double allowance{};

    double totalSalary() const {
        return basicSalary + hra + allowance;
    }
};
