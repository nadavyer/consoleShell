#include "../include/Environment.h"
#include <iostream>


Environment::Environment(): fs(), commandsHistory() {}

void Environment::start() {
    cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
    string input;
    getline(cin, input);
    while (input != "exit") {
        string cmdType = input.substr(0, input.find(' '));
        if (cmdType == "cd") {                                  // !todo debugged
            CdCommand *cd = new CdCommand(input);
            cd->execute(fs);
            addToHistory(cd);
        } else if (cmdType == "ls") {                            // !todo debugged
            LsCommand *ls = new LsCommand(input);
            ls->execute(fs);
            addToHistory(ls);
        } else if (cmdType == "pwd") {                       // !todo debugged
            PwdCommand *pwd = new PwdCommand(input);
            pwd->execute(fs);
            addToHistory(pwd);
        } else if (cmdType == "mkdir") {                        // !todo debugged
            MkdirCommand *mkdir = new MkdirCommand(input);
            mkdir->execute(fs);
            addToHistory(mkdir);
        } else if (cmdType == "mkfile") {                           // !todo debugged
            MkfileCommand *mkfile = new MkfileCommand(input);
            mkfile->execute(fs);
            addToHistory(mkfile);
        } else if (cmdType == "cp") {                                // !todo debugged
            CpCommand *cp = new CpCommand(input);
            cp->execute(fs);
            addToHistory(cp);
        } else if (cmdType == "mv") {                                // !todo debugged
            MvCommand *mv = new MvCommand(input);
            mv->execute(fs);
            addToHistory(mv);
        } else if (cmdType == "rename") {                           // !todo debugged
            RenameCommand *rename = new RenameCommand(input);
            rename->execute(fs);
            addToHistory(rename);
        } else if (cmdType == "rm") {                                // !todo debugged
            RmCommand *rm = new RmCommand(input);
            rm->execute(fs);
            addToHistory(rm);
        }
        else if (cmdType == "history") {                            // !todo debugged
            HistoryCommand *history = new HistoryCommand(input,commandsHistory);
            history->execute(fs);
            addToHistory(history);
        }
        else if (cmdType == "verbose") {                            //TODO: DO ME!!!!!
            VerboseCommand *verbose = new VerboseCommand(input);    //TODO: DO ME!!!!!
            verbose->execute(fs);                                   //TODO: DO ME!!!!!
            addToHistory(verbose);                                  //TODO: DO ME!!!!!
        }
        else if (cmdType == "exec") {                               // !todo debugged
            ExecCommand *exec = new ExecCommand(input,getHistory());
            exec->execute(fs);
            addToHistory(exec);
        }
        else {
            ErrorCommand *error = new ErrorCommand(input);          // !todo debugged
            error->execute(fs);
            addToHistory(error);
        }
        printWD();
        getline(cin,input);

    }

}

FileSystem &Environment::getFileSystem()  {
    return fs;
}


void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

const vector<BaseCommand *> &Environment::getHistory() const {
    return commandsHistory;
}

void Environment::printWD() {
    cout << fs.getWorkingDirectory().getAbsolutePath() + ">";
}

//copy constructor
Environment::Environment(const Environment &otherEnv): commandsHistory(),fs() {
    vector <BaseCommand*> :: const_iterator iter = otherEnv.commandsHistory.begin();
    for(iter; iter!=otherEnv.commandsHistory.end(); iter++){
        commandsHistory.push_back(*iter);
    }
    fs = FileSystem(otherEnv.fs);
}

//distructor
Environment::~Environment() {
    clear();
}

//move constructor
Environment::Environment(Environment &&other):commandsHistory(), fs() {
    commandsHistory = other.commandsHistory;
    fs = other.getFileSystem();
    other.commandsHistory.clear();
}



void Environment:: clear() {
    vector<BaseCommand *>::iterator iter = commandsHistory.begin();
    for (iter; iter != commandsHistory.end(); iter++) {
        delete (*iter);
    }
}

//copy assignment operator
Environment &Environment::operator=(const Environment &otherEnv) {
    if(this != &otherEnv){
        clear();
        commandsHistory = otherEnv.commandsHistory;
        fs = otherEnv.fs;
    }
    return *this;
}
//move assignment operator
Environment Environment::operator=(Environment &&otherEnv) {
    if(this != &otherEnv){
        clear();
        commandsHistory = otherEnv.commandsHistory;
        fs = otherEnv.getFileSystem();
        otherEnv.commandsHistory.clear();
    }
    return *this;
}