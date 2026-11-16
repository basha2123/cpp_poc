#include "EmployeeRepository.h"

#include <stdexcept>

namespace {
class Statement {
public:
    Statement(sqlite3* db, const char* sql) : db_(db) {
        const int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt_, nullptr);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db_));
        }
    }
    ~Statement() { sqlite3_finalize(stmt_); }
    sqlite3_stmt* get() const { return stmt_; }
private:
    sqlite3* db_;
    sqlite3_stmt* stmt_{nullptr};
};

void bindEmployee(sqlite3_stmt* stmt, const Employee& e) {
    sqlite3_bind_int(stmt, 1, e.id);
    sqlite3_bind_text(stmt, 2, e.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, e.age);
    sqlite3_bind_text(stmt, 4, e.department.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, e.designation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, e.basicSalary);
    sqlite3_bind_double(stmt, 7, e.hra);
    sqlite3_bind_double(stmt, 8, e.allowance);
}

Employee readEmployee(sqlite3_stmt* stmt) {
    Employee e;
    e.id = sqlite3_column_int(stmt, 0);
    e.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    e.age = sqlite3_column_int(stmt, 2);
    e.department = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    e.designation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    e.basicSalary = sqlite3_column_double(stmt, 5);
    e.hra = sqlite3_column_double(stmt, 6);
    e.allowance = sqlite3_column_double(stmt, 7);
    return e;
}
}

EmployeeRepository::EmployeeRepository(const std::string& databasePath) {
    const int rc = sqlite3_open(databasePath.c_str(), &db_);
    if (rc != SQLITE_OK) {
        const std::string message = db_ ? sqlite3_errmsg(db_) : "Unable to open database";
        if (db_) sqlite3_close(db_);
        db_ = nullptr;
        throw std::runtime_error(message);
    }
    createTable();
}

EmployeeRepository::~EmployeeRepository() {
    if (db_) sqlite3_close(db_);
}

void EmployeeRepository::checkDatabase(int result, const std::string& operation) const {
    if (result != SQLITE_OK) {
        throw std::runtime_error(operation + ": " + sqlite3_errmsg(db_));
    }
}

void EmployeeRepository::createTable() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS employees ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "age INTEGER NOT NULL CHECK(age BETWEEN 18 AND 65), "
        "department TEXT NOT NULL, "
        "designation TEXT NOT NULL, "
        "basic_salary REAL NOT NULL CHECK(basic_salary >= 0), "
        "hra REAL NOT NULL CHECK(hra >= 0), "
        "allowance REAL NOT NULL CHECK(allowance >= 0));";
    char* error = nullptr;
    const int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &error);
    if (rc != SQLITE_OK) {
        const std::string message = error ? error : sqlite3_errmsg(db_);
        sqlite3_free(error);
        throw std::runtime_error(message);
    }
}

bool EmployeeRepository::add(const Employee& e) {
    Statement statement(db_,
        "INSERT INTO employees VALUES(?, ?, ?, ?, ?, ?, ?, ?);");
    bindEmployee(statement.get(), e);
    const int rc = sqlite3_step(statement.get());
    if (rc == SQLITE_DONE) return true;
    if (rc == SQLITE_CONSTRAINT) return false;
    throw std::runtime_error(sqlite3_errmsg(db_));
}

std::optional<Employee> EmployeeRepository::findById(int id) const {
    Statement statement(db_,
        "SELECT id,name,age,department,designation,basic_salary,hra,allowance "
        "FROM employees WHERE id=?;");
    sqlite3_bind_int(statement.get(), 1, id);
    const int rc = sqlite3_step(statement.get());
    if (rc == SQLITE_ROW) return readEmployee(statement.get());
    if (rc == SQLITE_DONE) return std::nullopt;
    throw std::runtime_error(sqlite3_errmsg(db_));
}

std::vector<Employee> EmployeeRepository::getAll() const {
    Statement statement(db_,
        "SELECT id,name,age,department,designation,basic_salary,hra,allowance "
        "FROM employees ORDER BY id;");
    std::vector<Employee> employees;
    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement.get())) == SQLITE_ROW) {
        employees.push_back(readEmployee(statement.get()));
    }
    if (rc != SQLITE_DONE) throw std::runtime_error(sqlite3_errmsg(db_));
    return employees;
}

bool EmployeeRepository::update(const Employee& e) {
    Statement statement(db_,
        "UPDATE employees SET name=?,age=?,department=?,designation=?,"
        "basic_salary=?,hra=?,allowance=? WHERE id=?;");
    sqlite3_stmt* stmt = statement.get();
    sqlite3_bind_text(stmt, 1, e.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, e.age);
    sqlite3_bind_text(stmt, 3, e.department.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, e.designation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, e.basicSalary);
    sqlite3_bind_double(stmt, 6, e.hra);
    sqlite3_bind_double(stmt, 7, e.allowance);
    sqlite3_bind_int(stmt, 8, e.id);
    const int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) throw std::runtime_error(sqlite3_errmsg(db_));
    return sqlite3_changes(db_) > 0;
}

bool EmployeeRepository::remove(int id) {
    Statement statement(db_, "DELETE FROM employees WHERE id=?;");
    sqlite3_bind_int(statement.get(), 1, id);
    const int rc = sqlite3_step(statement.get());
    if (rc != SQLITE_DONE) throw std::runtime_error(sqlite3_errmsg(db_));
    return sqlite3_changes(db_) > 0;
}
