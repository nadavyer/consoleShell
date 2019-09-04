#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class BaseFile {
private:
    string name;

public:
    BaseFile(string name);

    string getName() const ;

    void setName(string newName);

    virtual int getSize() =0;

    virtual const bool getIsDir() =0;

    virtual ~BaseFile(); //destructor

};

class File : public BaseFile {
private:
    int size;


public:
    File(string name, int size); // Constructor
    int getSize(); // Return the size of the file
    const bool getIsDir();


};

class Directory : public BaseFile {
private:
    vector<BaseFile*> children;
    Directory* parent;

public:
    Directory(string name, Directory *parent); // Constructor
    Directory(const Directory &otherDirectory); //copy constructor
    Directory &operator=(const Directory &rhs);//copy assignment operator
    ~Directory(); //destructor
    Directory (Directory &&stealFromDir); //move constructor
    Directory operator= (Directory &&stealFromDir); //move assignment operator
    void clean();
    void copy(const Directory &other);
    void removePointerOnly(BaseFile *toRm);



    Directory *getParent() const; // Return a pointer to the parent of this directory
    void setParent(Directory *newParent); // Change the parent of this directory
    void addFile(BaseFile *file); // Add the file to children
    void removeFile(string name); // Remove the file with the specified name from children
    void removeFile(BaseFile *file); // Remove the file from children
    void sortByName(); // Sort children by name alphabetically (not recursively)
    void sortBySize(); // Sort children by size (not recursively)
    vector<BaseFile *> getChildren() const; // Return children
    int getSize(); // Return the size of the directory (recursively)


    string getAbsolutePath();  //Return the path from the root to this
    const bool getIsDir();
    static bool comperatorByName(BaseFile *a, BaseFile *b); //comperator for name sorting
    static bool comperatorBySize(BaseFile *a, BaseFile *b); //comperaor for sort by size
    bool getIfHasNameInChildrenByName(string name);
    BaseFile * getChildByName(string &name);
    int indxOfChild(BaseFile *Child);

};

#endif