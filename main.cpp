#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

#define ID_SIZE 8
#define BIO_SIZE 200
#define NAME_SIZE 500
#define SPLIT_PARAMETER 0.8

using namespace std;

class Record{
public:
    string id;
    string managerId;
    string bio;
    string name;

    Record(string id, string managerId, string bio, string name);
};

class Block{
public:
    vector<Record> records;

    void insert(Record);
};

class HashTable{
public:

};

Record::Record(string id, string name, string bio, string managerId){
    // copy values into bucket up to the maximum size
    this->id = id.substr(0, ID_SIZE);
    this->managerId = managerId.substr(0, ID_SIZE);
    this->bio = bio.substr(0, BIO_SIZE);
    this->name = name.substr(0, NAME_SIZE);
}


vector<Record> getRecordsFromFile(string filename){
    vector<Record> records;
    string id, name, bio, managerId;

    ifstream file(filename.c_str());

    while(getline(file, id, ',')){
        getline(file, name, ',');
        getline(file, bio, ',');
        getline(file, managerId, '\n');
        records.push_back(Record(id, name, bio, managerId));
    }
    return records;
}

int main(int argc, char* argv[]){
    vector<Record> records = getRecordsFromFile("Employees.csv");
    for(int i = 0; i < records.size(); i++){
        cout << "id: " << records[i].id << endl;
        cout << "name: " << records[i].name << endl;
        cout << "bio: " << records[i].bio << endl;
        cout << "managerId: " << records[i].managerId << endl;
        cout << "------------------------" << endl;
    }
}

