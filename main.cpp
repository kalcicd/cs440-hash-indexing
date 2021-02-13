#include <iostream>
#include <string>
#define ID_SIZE 8
#define BIO_SIZE 200
#define NAME_SIZE 500
using namespace std;

class Bucket{
public:
    char* id;
    char* managerId;
    char* bio;
    char* name;
    Bucket(char* id, char* managerId, char* bio, char* name);

    virtual ~Bucket();
};

Bucket::~Bucket(){
    free(this->id);
    free(this->managerId);
    free(this->bio);
    free(this->name);
}

Bucket::Bucket(char* id, char* managerId, char* bio, char* name){
    // allocate memory for each member
    this->id = (char*)malloc(ID_SIZE);
    this->managerId = (char*)malloc(ID_SIZE);
    this->bio = (char*)malloc(BIO_SIZE);
    this->name = (char*)malloc(NAME_SIZE);

    // copy values into bucket
    snprintf(this->id, ID_SIZE, "%s", id);
    snprintf(this->managerId, ID_SIZE, "%s", managerId);
    snprintf(this->bio, BIO_SIZE, "%s", bio);
    snprintf(this->name, NAME_SIZE, "%s", name);
}


int main(int argc, char* argv[]){
    Bucket lasagna = Bucket("1", "2", "I'm a cat who loves to snooze", "Garfielf");
    cout << "id: " << lasagna.id << endl;
    cout << "name: " << lasagna.name << endl;
    cout << "bio: " << lasagna.bio << endl;
    cout << "managerId: " << lasagna.managerId << endl;
}

