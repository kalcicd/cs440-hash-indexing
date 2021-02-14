#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <math.h>

#define ID_SIZE 8
#define BIO_SIZE 200
#define NAME_SIZE 500
#define SPLIT_PARAMETER 0.8

using namespace std;

class Employee{
public:
    string id;
    string managerId;
    string bio;
    string name;

    Employee(string id, string managerId, string bio, string name);
};

Employee::Employee(string id, string name, string bio, string managerId){
    // copy values into bucket up to the maximum size
    this->id = id.substr(0, ID_SIZE);
    this->managerId = managerId.substr(0, ID_SIZE);
    this->bio = bio.substr(0, BIO_SIZE);
    this->name = name.substr(0, NAME_SIZE);
}

class Block{
public:
    vector<Employee> employees;

    void insert(Employee);
};

class HashTable{
public:
    int n; //n = current block
    int modfactor; //[original number of blocks]*2^[number of blocks]
    
    vector<Block> blocks;

    void insert(Employee); //checks if block has space, if not: splits first
    void split(int, Employee); //creates a new block and distributes data
};

void HashTable::insert(Employee emp) {
    int modfactor = pow(2, blocks.size());
    int id = stoi(emp.id);
    int index = id % modfactor;

    if(sizeof(blocks[index]) + sizeof(emp) > 4096) {
        //overflow, make a overflow block
        int numEmps = 0;
        for(int ii = 0; ii < blocks.size(); ii++) {
            for(int jj = 0; jj < blocks[ii].employees[jj]; jj ++) {
                numEmps++;
            }
        }
        split(index, emp);
    } else {
        blocks[index].employees[sizeof(blocks[index])] = emp;
    }
}

void HashTable::split(int index, Employee emp) {
    cout << "shout out bananas cuz ima split";
}

vector<Employee> getemployeesFromFile(string filename){
    vector<Employee> employees;
    string id, name, bio, managerId;

    ifstream file(filename.c_str());

    while(getline(file, id, ',')){
        getline(file, name, ',');
        getline(file, bio, ',');
        getline(file, managerId, '\n');
        employees.push_back(Employee(id, name, bio, managerId));
    }
    return employees;
}

int main(int argc, char* argv[]){
    vector<Employee> employees = getemployeesFromFile("Employees.csv");
    for(int i = 0; i < employees.size(); i++){
        cout << "id: " << employees[i].id << endl;
        cout << "name: " << employees[i].name << endl;
        cout << "bio: " << employees[i].bio << endl;
        cout << "managerId: " << employees[i].managerId << endl;
        cout << "------------------------" << endl;
    }
    cout << "There were " << employees.size() << " employees in total" << endl;
}

