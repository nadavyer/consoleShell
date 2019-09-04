#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "../include/FileSystem.h"
#include "../include/Commands.h"
#include "../include/Files.h"
#include "../include/GlobalVariables.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
    vector<BaseCommand*> commandsHistory;
    FileSystem fs;
    void printWD();


public:                                                              //TODO: 5 rule
    Environment();
    void start();
    FileSystem& getFileSystem(); // Get a reference to the file system
    void addToHistory(BaseCommand *command); // Add a new command to the history
    const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
    Environment(const Environment &otherEnvironment); //copy constructor
    Environment &operator=(const Environment &otherEnv);//copy assignment operator
    virtual ~Environment(); //destructor
    Environment (Environment &&stealFromEnv); //move constructor
    Environment operator= (Environment &&otherEnv); //move assignment operator
    void clear();

};

#endif