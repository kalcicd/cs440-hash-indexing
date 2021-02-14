#include <iostream>
#include <string>
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
    string id, name, bio, managerId;

    Employee(string id, string name, string bio, string managerId);

    int getSize();
};

Employee::Employee(string id, string name, string bio, string managerId){
    // copy values into bucket up to the maximum size
    this->id = id.substr(0, ID_SIZE);
    this->managerId = managerId.substr(0, ID_SIZE);
    this->bio = bio.substr(0, BIO_SIZE);
    this->name = name.substr(0, NAME_SIZE);
}

int Employee::getSize(){
    return id.size() + name.size() + bio.size() + managerId.size();
}

class Block{
public:
    vector<Employee> employees;

    void insert(Employee);

    int getTotalSize();
};

void Block::insert(Employee emp){
// make sure to enforce block size
}

int Block::getTotalSize(){
    int sum = 0;
    for(int ii = 0; ii < employees.size(); ii++){
        sum += employees[ii].getSize();
    }
    return sum;
}

class HashTable{
public:
    int n; //n = current block

    vector<Block> blocks;

    void insert(Employee); //checks if block has space, if not: splits first
    void split(int, Employee); //creates a new block and distributes data
    int getTableSize();
};

int HashTable::getTableSize(){
    int sum = 0;
    for (int ii = 0; ii < blocks.size(); ii++){
        sum += blocks[ii].getTotalSize();
    }
    return sum;
}

void HashTable::insert(Employee emp){
    int modFactor = pow(2, blocks.size());
    int id = stoi(emp.id);
    int index = id % modFactor;

    if((blocks[index].getTotalSize() + emp.getSize()) > 4096){
        //overflow, make a overflow block
        int numEmps = 0;
        for(int ii = 0; ii < blocks.size(); ii++){
            for(int jj = 0; jj < blocks[ii].employees.size(); jj++){
                numEmps++;
            }
        }
        // total size of all emps / (number of blocks * 4096) > 0.8
        if(((float)getTableSize() / (float)(blocks.size() * 4096)) > SPLIT_PARAMETER){
            split(index, emp);
        }
    } else{
        blocks[index].employees.push_back(emp);
    }
}

void HashTable::split(int index, Employee emp){
    cout << "shout out bananas cuz ima split";
}

vector<Employee> getEmployeesFromFile(string filename){
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
    HashTable h;
    vector<Employee> employees = getEmployeesFromFile("Employees.csv");
    for(int i = 0; i < employees.size(); i++){
        cout << "Inserting employee. . ." << endl;
        cout << "id: " << employees[i].id << endl;
        cout << "name: " << employees[i].name << endl;
        cout << "bio: " << employees[i].bio << endl;
        cout << "managerId: " << employees[i].managerId << endl;
        h.insert(employees[i]);
        cout << "------------------------" << endl;
    }
    cout << "There are " << employees.size() << " employees in total" << endl;
}

