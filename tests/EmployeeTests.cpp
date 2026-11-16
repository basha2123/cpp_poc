#include "EmployeeService.h"

#include <gtest/gtest.h>
#include <cstdio>
#include <string>

class EmployeeServiceTest : public ::testing::Test {
protected:
    std::string dbPath = "employee_test.db";

    void SetUp() override {
        std::remove(dbPath.c_str());
        repository = new EmployeeRepository(dbPath);
        service = new EmployeeService(*repository);
    }

    void TearDown() override {
        delete service;
        delete repository;
        std::remove(dbPath.c_str());
    }

    EmployeeRepository* repository{nullptr};
    EmployeeService* service{nullptr};

    Employee sample() {
        return Employee{101, "Rahul", 25, "Engineering",
                        "Software Engineer", 30000.0, 5000.0, 3000.0};
    }
};

TEST_F(EmployeeServiceTest, AddAndSearchEmployee) {
    std::string error;
    ASSERT_TRUE(service->addEmployee(sample(), error)) << error;
    const auto found = service->getEmployee(101);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->name, "Rahul");
}

TEST_F(EmployeeServiceTest, RejectDuplicateEmployeeId) {
    std::string error;
    ASSERT_TRUE(service->addEmployee(sample(), error));
    EXPECT_FALSE(service->addEmployee(sample(), error));
    EXPECT_EQ(error, "Employee ID already exists");
}

TEST_F(EmployeeServiceTest, RejectInvalidAge) {
    std::string error;
    Employee e = sample();
    e.age = 17;
    EXPECT_FALSE(service->addEmployee(e, error));
    EXPECT_EQ(error, "Age must be between 18 and 65");
}

TEST_F(EmployeeServiceTest, RejectNegativeSalary) {
    std::string error;
    Employee e = sample();
    e.basicSalary = -1.0;
    EXPECT_FALSE(service->addEmployee(e, error));
}

TEST_F(EmployeeServiceTest, UpdateEmployee) {
    std::string error;
    ASSERT_TRUE(service->addEmployee(sample(), error));
    Employee changed = sample();
    changed.name = "Rahul Kumar";
    ASSERT_TRUE(service->updateEmployee(changed, error)) << error;
    EXPECT_EQ(service->getEmployee(101)->name, "Rahul Kumar");
}

TEST_F(EmployeeServiceTest, DeleteEmployee) {
    std::string error;
    ASSERT_TRUE(service->addEmployee(sample(), error));
    EXPECT_TRUE(service->deleteEmployee(101));
    EXPECT_FALSE(service->getEmployee(101).has_value());
}

TEST_F(EmployeeServiceTest, MissingEmployeeReturnsNoRecord) {
    EXPECT_FALSE(service->getEmployee(999).has_value());
}

TEST_F(EmployeeServiceTest, CalculateTotalSalary) {
    const Employee e = sample();
    EXPECT_DOUBLE_EQ(e.totalSalary(), 38000.0);
}

TEST_F(EmployeeServiceTest, ListAllEmployees) {
    std::string error;
    ASSERT_TRUE(service->addEmployee(sample(), error));
    Employee second = sample();
    second.id = 102;
    second.name = "Priya";
    ASSERT_TRUE(service->addEmployee(second, error));
    EXPECT_EQ(service->getAllEmployees().size(), 2U);
}
