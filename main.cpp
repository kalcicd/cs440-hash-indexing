#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

#define ID_SIZE 8
#define BIO_SIZE 200
#define NAME_SIZE 500
#define BLOCK_SIZE 4096
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

    vector<Employee> employees, overflow;
    int modFactor;
    void insert(Employee);
    int getTotalSize();
    Block();
    Block(int);
};

Block::Block(){
    modFactor = 2;
}

Block::Block(int modFactor){
    this->modFactor = modFactor;
}

void Block::insert(Employee emp){

    // make sure to enforce block size (create overflow if already full)
    if(getTotalSize() + emp.getSize() > BLOCK_SIZE){
        cout << "Inserting " << emp.id << " into overflow" << endl;
        overflow.push_back(emp);
    } else {
        cout << "Inserting " << emp.id << " into employees" << endl;
        employees.push_back(emp);
    }
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
    HashTable();
    void insert(Employee); //checks if block has space, if not: splits first
    void split(int); //creates a new block and distributes data
    int getTableSize();
};

HashTable::HashTable(){
    n = 0;
    blocks = {{}, {}};
}

int HashTable::getTableSize(){
    int sum = 0;
    for (int ii = 0; ii < blocks.size(); ii++){
        sum += blocks[ii].getTotalSize();
    }
    return sum;
}

void HashTable::insert(Employee emp){
    int modFactor = pow(2, blocks.size() - 1);
    int id = stoi(emp.id);
    int index = id % modFactor;

    if((blocks[index].getTotalSize() + emp.getSize()) > BLOCK_SIZE){
        //overflow, make a overflow block
        int numEmps = 0;
        for(int ii = 0; ii < blocks.size(); ii++){
            for(int jj = 0; jj < blocks[ii].employees.size(); jj++){
                numEmps++;
            }
        }
        // total size of all emps / (number of blocks * 4096) > 0.8
        if(((float)getTableSize() / (float)(blocks.size() * BLOCK_SIZE)) > SPLIT_PARAMETER){
            split(index);
        }
    } else{
        blocks[index].insert(emp);
    }
}

void HashTable::split(int index){
    // This calculates the modFactor
    int modFactor = pow(2, ceil(log2(blocks.size() + 1)));
    int newIndex = 0, overflowSize = 0;
    int blockSize = blocks[n].employees.size();

    blocks.push_back(Block(modFactor));
    blocks[n].modFactor = modFactor;

    for (int ii = 0; ii < blockSize; ii++){
        newIndex = stoi(blocks[n].employees.back().id) % blocks[n].modFactor;
        if (newIndex != n){
            // check for overflow here
            blocks[newIndex].insert(blocks[n].employees.back());
            blocks[n].employees.pop_back();

            //employee[ii] out of blocks[n] into blocks[newIndex]
        }
    }

    if (!blocks[n].overflow.empty()){
        overflowSize = blocks[n].overflow.size();
        for (int ii = 0; ii < overflowSize; ii++){
            newIndex = stoi(blocks[n].overflow.back().id) % blocks[n].modFactor;
            if (newIndex != n){
                // check for overflow here
                blocks[newIndex].insert(blocks[n].overflow.back());
                blocks[n].overflow.pop_back();
            } else {
                blocks[n].insert(blocks[n].overflow.back());
                blocks[n].overflow.pop_back();
            }
        }
    }


    if(blocks[n].modFactor == blocks[n + 1].modFactor){
        n++;
    } else {
        n = 0;
    }
    cout << "shout out bananas cuz ima split" << endl;
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
    /*
    for(int i = 0; i < employees.size(); i++){
        cout << "Inserting employee. . ." << endl;
        cout << "id: " << employees[i].id << endl;
        cout << "name: " << employees[i].name << endl;
        cout << "bio: " << employees[i].bio << endl;
        cout << "managerId: " << employees[i].managerId << endl;
        h.insert(employees[i]);
        cout << "------------------------" << endl;
    } */
    Employee garf1 = Employee("1", "Garfielf", "I'm a cat that likes lasagna", "2");
    Employee garf2 = Employee("2", "Garfielf", "I'm a cat that likes lasagna", "2");
    Employee garf3 = Employee("3", "Garfielf", "I'm a cat that likes lasagna", "2");
    Employee garf4 = Employee("4", "Garfielf", "I'm a cat that likes lasagna", "2");
    Employee garf5 = Employee("5", "Garfielf", "I'm a cat that likes lasagna", "2");
    Employee garf6 = Employee("6", "Garfielf", "I'm a cat that likes lasagna", "2");

    for(int ii = 0; ii < 108; ii++){
        h.insert(garf1);
        h.insert(garf2);
    }


    //cout << "GARF1 SIZE: " << garf1.getSize() << endl;

    cout << "There are " << employees.size() << " employees in total" << endl;
}

