#CS 440 HW #3 Linear Hash Indexing
##Authors: David Kalcic, Zachary Kauffman
To compile:

`g++ -std=c++11 main.cpp -o main`

NOTE: Program expects the name of the employee CSV to be "Employees.csv"
If your file name is "Employee.csv" please go to line 208 and change the getEmployeesFromFile() parameter appropriately

To create the employee index:

`./main C`

To locate an employee by ID:

`./main L [ID]`