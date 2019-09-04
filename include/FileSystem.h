#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "../include/Files.h"
//#include "../include/Commands.h"




class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
public:
	FileSystem(); //constructor
	virtual ~FileSystem(); //destructor
	void clean();
	FileSystem(const FileSystem &other);//copy constructor
	FileSystem &operator=(const FileSystem &other);//copy assignment opp
	FileSystem (FileSystem &&other); //move constructor
	FileSystem &operator= (FileSystem &&other); //move assignment operator
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system
    Directory* getNewWD(Directory *otherWD , Directory &newRoot);
    void copy (const FileSystem &other);
    vector <string> pathStringToVec(string givenPath);

};

#endif