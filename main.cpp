//Zach Kauffman and David Kalcic
//For OSU CS-440 Winter Term 2021

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

//employee class contains info for the employee
class Employee{
public:
    string id, name, bio, managerId;

    Employee(string id, string name, string bio, string managerId);

    int getSize();
};

Employee::Employee(string id, string name, string bio, string managerId){
    //store values only up to maximum size
    this->id = id.substr(0, ID_SIZE);
    this->managerId = managerId.substr(0, ID_SIZE);
    this->bio = bio.substr(0, BIO_SIZE);
    this->name = name.substr(0, NAME_SIZE);
}

//returns the size of the employee in bytes
int Employee::getSize(){
    return id.length() + name.length() + bio.length() + managerId.length();
}

//a block holds employees up to 4096 bytes
class Block{
public:

    //contains a vector for employees up to 4096 bytes
    //and an overflow vector for employees past that size
    vector<Employee> employees, overflow;
    int modFactor;

    void insert(Employee);

    int getTotalSize();
    int getSizeNoOverflow();

    Block();
    Block(int);
};

//default constructor
Block::Block(){
    modFactor = 2;
}

Block::Block(int modFactor){
    this->modFactor = modFactor;
}

//inserts an employee into the block or overflow depending on remaining space
void Block::insert(Employee emp){
    //checks if current size + employee size will go past 4096 bytes
    if(getSizeNoOverflow() + emp.getSize() > BLOCK_SIZE){
        overflow.push_back(emp);
    } else{
        employees.push_back(emp);
    }
}

//get the total size of just non-overflow employees in the block
int Block::getSizeNoOverflow(){
    int sum = 0;
    for(int ii = 0; ii < employees.size(); ii++){
        sum += employees[ii].getSize();
    }
    return sum;
}

//get the total size of all employees in block and overflow
int Block::getTotalSize(){
    int sum = getSizeNoOverflow();
    if(!overflow.empty()){
        for(int ii = 0; ii < overflow.size(); ii++){
            sum += overflow[ii].getSize();
        }
    }
    return sum;
}

//hashtable holds blocks
class HashTable{
public:
    int n; //n = current block

    vector<Block> blocks;

    HashTable();

    void insert(Employee); //checks if block has space, if not: splits first
    void split(); //creates a new block and distributes data
    int getTableSize();
};

//default constructor
//contains two empty blocks by default
HashTable::HashTable(){
    n = 0;
    blocks = {{},
              {}};
}

//returns total size of contents of all blocks, including overflow
int HashTable::getTableSize(){
    int sum = 0;
    for(int ii = 0; ii < blocks.size(); ii++){
        sum += blocks[ii].getTotalSize();
    }
    return sum;
}

//finds which block to insert an employee into and then inserts it
//if block is full, inserts employee into overload
//if split parameter is hit (80%), split at index n
//if split is performed, increment n
void HashTable::insert(Employee emp){
    //calculates current mod factor
    int modFactor = pow(2, ceil(log2(blocks.size())));
    int id = stoi(emp.id);
    int index = id % modFactor; //calculates proper index for the employee by id mod modfactor

    //ensures modfactor doesn't get too big 
    //i don't know why i had to do this
    if(index >= blocks.size()){
        index = id % (modFactor / 2);
    }

    //inserts employee at proper index (or overflow)
    blocks[index].insert(emp);

    //checks if split parameter has been hit
    if(((float) getTableSize() / (float) (blocks.size() * BLOCK_SIZE)) > SPLIT_PARAMETER){
        split();
    }
}

void HashTable::split(){
    //modfactor for the split needs to be calculated at current size + 1, since a new block will be added
    int modFactor = pow(2, ceil(log2(blocks.size() + 1)));
    int newIndex = 0, overflowSize = 0;
    int blockSize = blocks[n].employees.size(); //number of employees

    blocks.push_back(Block(modFactor));//insert a new block
    blocks[n].modFactor = modFactor;//increment modfactor at block n

    //redistributes employees from block n as necessary
    for(int ii = 0; ii < blockSize; ii++){
        newIndex = stoi(blocks[n].employees.back().id) % blocks[n].modFactor;
        if(newIndex != n){
            blocks[newIndex].insert(blocks[n].employees.back());
            //employee[ii] out of blocks[n] into blocks[newIndex]
        } else{
            blocks[n].employees.insert(blocks[n].employees.begin(), blocks[n].employees.back());
        }
        blocks[n].employees.pop_back();
    }

    //if there are employees in overflow, redistribute them
    if(!blocks[n].overflow.empty()){
        overflowSize = blocks[n].overflow.size();
        for(int ii = 0; ii < overflowSize; ii++){
            newIndex = stoi(blocks[n].overflow.back().id) % blocks[n].modFactor;
            if(newIndex != n){
                // check for overflow here
                blocks[newIndex].insert(blocks[n].overflow.back());
                blocks[n].overflow.pop_back();
            } else{
                blocks[n].insert(blocks[n].overflow.back());
                blocks[n].overflow.pop_back();
            }
        }
    }

    //more modfactor jank
    blocks[n].modFactor /= 2;

    //increments modfactor to either n+1 or 0
    if(blocks[n].modFactor == blocks[n + 1].modFactor){
        n++;
    } else{
        n = 0;
    }
    blocks[n].modFactor = modFactor; //the last jank i promise

}

//reads employee data from a csv and returns a vector of the Employee class
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
    if(string(argv[1]) == "C"){
        ofstream myfile("EmployeeIndex.txt");
        if(myfile.is_open()){
            for(int i = 0; i < employees.size(); i++){
                h.insert(employees[i]);
            }
            for(int i = 0; i < h.blocks.size(); i++){
                myfile << "Block " << i << ":\n";
                for(int j = 0; j < h.blocks[i].employees.size(); j++){
                    myfile << "--id:" << h.blocks[i].employees[j].id << endl;
                    myfile << "--name:" << h.blocks[i].employees[j].name << endl;
                    myfile << "--bio:" << h.blocks[i].employees[j].bio << endl;
                    myfile << "--managerId:" << h.blocks[i].employees[j].managerId << endl << endl;
                }
                myfile << endl;
            }
            myfile.close();
            cout << "Wrote to EmployeeIndex.txt\n";
            return 0;
        } else {
            cout << "Error opening file\n";
            return 1;
        }
    }
    if(string(argv[1]) == "L"){
        if(argc != 3){
            cout << "MISSING ID ARG" << endl;
        } else{
            cout << "Locating employee...\n";
            for(int ii = 0; ii < employees.size(); ii++){
                if(employees[ii].id == string(argv[2])){
                    cout << "Employee id: " << employees[ii].id << endl;
                    cout << "Name: " << employees[ii].name << endl;
                    cout << "Bio: " << employees[ii].bio << endl;
                    cout << "Manager id: " << employees[ii].managerId << endl;
                    return 0;
                }
            }
            cout << "Couldn't find an employee with that ID\n";
            return 1;
        }
    }
}
