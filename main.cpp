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
    return id.length() + name.length() + bio.length() + managerId.length();
}

class Block{
public:

    vector<Employee> employees, overflow;
    int modFactor;

    void insert(Employee);

    int getTotalSize();

    int getSizeNoOverflow();

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
    cout << "---block size: " << getSizeNoOverflow() << endl;
    cout << "---emp size: " << emp.getSize() << endl;
    // make sure to enforce block size (create overflow if already full)
    if(getSizeNoOverflow() + emp.getSize() > BLOCK_SIZE){
        cout << "---Inserting " << emp.id << " into overflow" << endl;
        overflow.push_back(emp);
    } else{
        cout << "---Inserting " << emp.id << " into employees" << endl;
        employees.push_back(emp);
    }
}

int Block::getSizeNoOverflow(){
    int sum = 0;
    for(int ii = 0; ii < employees.size(); ii++){
        sum += employees[ii].getSize();
    }
    return sum;
}

int Block::getTotalSize(){
    int sum = getSizeNoOverflow();
    if(!overflow.empty()){
        for(int ii = 0; ii < overflow.size(); ii++){
            sum += overflow[ii].getSize();
        }
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
    blocks = {{},
              {}};
}

int HashTable::getTableSize(){
    int sum = 0;
    for(int ii = 0; ii < blocks.size(); ii++){
        sum += blocks[ii].getTotalSize();
    }
    return sum;
}

void HashTable::insert(Employee emp){
    int modFactor = pow(2, ceil(log2(blocks.size())));
    cout << "Mod factor " << modFactor << endl;
    int id = stoi(emp.id);
    int index = id % modFactor;

    if(index >= blocks.size()){
        cout << "Just kidding... Mod factor " << modFactor / 2 << endl;
        index = id % (modFactor / 2);
    }

    cout << "Inserting in block #" << index << " ...\n";
    blocks[index].insert(emp);

    if(((float) getTableSize() / (float) (blocks.size() * BLOCK_SIZE)) > SPLIT_PARAMETER){
        split(index);
    }
}

void HashTable::split(int index){
    // This calculates the modFactor
    int modFactor = pow(2, ceil(log2(blocks.size() + 1)));
    int newIndex = 0, overflowSize = 0;
    int blockSize = blocks[n].employees.size();

    cout << "|||Splitting on block " << n << endl;

    blocks.push_back(Block(modFactor));
    blocks[n].modFactor = modFactor;

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

    blocks[n].modFactor /= 2;

    cout << "Your new n is ... ";
    if(blocks[n].modFactor == blocks[n + 1].modFactor){
        n++;
    } else{
        n = 0;
    }
    cout << n << "!!!\n";

    blocks[n].modFactor = modFactor;

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
    if(string(argv[1]) == "C"){
        ofstream myfile("EmployeeIndex");
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
        }


    }
    if(argv[1] == "L"){
        if(argc != 3){
            cout << "MISSING ID ARG" << endl;
        } else{
            for(int ii = 0; ii < employees.size(); ii++){
                if(employees[ii].id == string(argv[2])){
                    cout << "Employee id: " << employees[ii].id << endl;
                    cout << "Name: " << employees[ii].name << endl;
                    cout << "Bio: " << employees[ii].bio << endl;
                    cout << "Manager id: " << employees[ii].managerId << endl;
                }
            }
        }
    }
}

