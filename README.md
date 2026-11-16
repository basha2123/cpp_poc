# Employee Management System – C++ POC

## Scope
Console-based employee CRUD application using C++17, OOP and SQLite.
Includes Google Test tests and a tool-based quality workflow.

## Features
- Add, list, search, update and delete employees
- Validate employee ID, name, age, department, designation and salary components
- Calculate total salary
- Persist records in `employee.db`

## Dependencies (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake libsqlite3-dev libgtest-dev cppcheck valgrind lcov
```
A MISRA checker/add-on may need to be installed separately according to your organization's approved toolchain.

## Build and run
```bash
cmake -S . -B build
cmake --build build -j
./build/employee_app
```

## Run Google Test
```bash
ctest --test-dir build --output-on-failure
```

## Static analysis
```bash
cppcheck --enable=all --inconclusive --std=c++17 --language=c++ \
  --suppress=missingIncludeSystem -I include src tests
```

## Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes ./build/employee_tests
```

## Optional Helgrind
```bash
valgrind --tool=helgrind ./build/employee_tests
```
Helgrind is most useful when the application actually uses multiple threads. This POC is currently single-threaded.

## Code coverage
```bash
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage -j
ctest --test-dir build-coverage --output-on-failure
lcov --capture --directory build-coverage --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_report
```
Open `coverage_report/index.html`.

## MISRA
MISRA compliance is not established merely by running Cppcheck. Use the approved MISRA C++ checker and rule set for your project. Record tool/version, rule set, deviations, violations and disposition. Cppcheck's MISRA addon, where available and licensed/approved, provides a subset of checks; it is not proof of full compliance.

## Notes
- The database file is created automatically on first run.
- This is a POC baseline, not a production HR/payroll system.
- For production, add authentication/authorization, audit logging, database backup, transaction handling and stronger input handling.
