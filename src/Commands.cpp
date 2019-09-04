//
// Created by nadav on 09/11/17.
//

#include "../include/Commands.h"
#include "../include/GlobalVariables.h"

using namespace std;

#include <string>
#include <iostream>
#include <vector>

BaseCommand::BaseCommand(string args) : args(args) {}

BaseCommand::~BaseCommand() {};

string BaseCommand::getArgs() {
    return args;
}

// accepts only if command also in givenPath string
vector<string> BaseCommand::pathStringToVec(string givenPath) {
    vector<string> path;
    string str = givenPath;

    if (givenPath == "/") {
        path.push_back("/");
        return path;
    }

    // remove command
    if (givenPath.find(' ') != string::npos) {
        str = givenPath.substr(givenPath.find(' ') + 1);
    }

    vector<string> pathParts = BaseCommand::devideStringByChar(str, '/');

    if (str.at(0) == '/') {
        path.emplace_back("/");
    }

    for (string part : pathParts) {
        path.push_back(part);
    }

    return path;
}

// return pointer to basefile by string ex: a/b/c -> return c
BaseFile *BaseCommand::hasValidPathGetLast(FileSystem &fs, string givenPath) {


    Directory *p = &fs.getWorkingDirectory();
    BaseFile *curChild = nullptr;
    // remove command
    if (givenPath.find(' ') != string::npos) {
        givenPath = givenPath.substr(givenPath.find(' ') + 1);
    }


    vector<string> path = BaseCommand::devideStringByChar(givenPath, '/');

    if (givenPath.at(0) == '/') {
        p = &fs.getRootDirectory();
    } else {
        p = &fs.getWorkingDirectory();
    }


    if (givenPath == "/") {
        return &fs.getRootDirectory();
    }
    size_t i;

    for (i = 0; i < path.size() - 1; i++) {
        // ------------------------------------  handle <..>
        if (path.at(i) == "..") {
            if (p->getParent() == nullptr) {
                return nullptr;
            } else {
                p = p->getParent();
            }
            // ------------------------------------ otherwise
        } else {

            curChild = p->getChildByName(path.at(i));

            if (!curChild->getIsDir()) {
                return nullptr;
            }

            p = dynamic_cast<Directory *>(curChild);
        }
    }


    if (path.at(i) == ".." && p->getParent()) {
        return p->getParent();
    } else if (path.at(i) == "..") {
        return nullptr;
    } else {
        // bellow will return nullptr if child doesnt exist
        return p->getChildByName(path.at(i));
    }

}

vector<string> BaseCommand::pathToVecByChar(string givenPath, char x) {
    vector<string> path;
    string str = getArgs().substr(givenPath.find('x') + 1);
    if (str.at(0) == '/') {
        path.push_back("/");
    }
    string word = "";
    while (str != "") {
        int i;
        for (i = 0; str.at(i) != 'x'; i++) {
            word = word + str.at(i);
        }
        str = str.substr(i + 1);
        path.push_back(word);
        word = "";
    }

    return path;
}

vector<string> BaseCommand::devideStringByChar(string givenArgs, char x) {
    vector<string> path;
    string word = "";

    if (givenArgs.size() > 1 && givenArgs.at(0) == x) {
        givenArgs = givenArgs.substr(1);
    }

    while (givenArgs != "") {
        size_t i;
        for (i = 0; i < givenArgs.size() && givenArgs.at(i) != x; i++) {
            word = word + givenArgs.at(i);
        }
        if (i < givenArgs.size()) {
            givenArgs = givenArgs.substr(i + 1);
        } else {
            givenArgs = "";
        }
        path.push_back(word);
        word = "";

    }

    return path;
}

Directory *BaseCommand::returnParent(FileSystem &fs, string path) {
    vector<string> devideCmdBySpace = devideStringByChar(path, ' ');
    string str = (string) devideCmdBySpace.at(1);
    vector<string> pathByVec = pathStringToVec(str);
    string last = (string) pathByVec.at(pathByVec.size() - 1);
    string newPath = path.substr(0, path.length() - last.length() - 1); //-1 for the "/"
    Directory *parentOfLast = (Directory *) hasValidPathGetLast(fs, newPath);
    return parentOfLast;

}

string BaseCommand::genStringFromPathVector(vector<string> &pathVec) {
    string pathS = "";
    for (string pvPart : pathVec) {
        pathS = pathS + "/" + pvPart;
    }

    while (pathS.size() >= 2 && pathS.at(0) == '/' && pathS.at(1) == '/') {
        pathS = pathS.substr(1);
    }

    return pathS;
}

bool BaseCommand::isWorkingDirOrItsAncestor(FileSystem &fs, Directory *suspect) {
    Directory *curDir = &fs.getWorkingDirectory();
    while (curDir) {
        if (suspect == curDir) {
            return true;
        }
        curDir = curDir->getParent();
    }

    return false;
}


PwdCommand::PwdCommand(string args) : BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() {
    return "pwd";
}


CdCommand::CdCommand(string args) : BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() << endl;
    }
    BaseFile *latestInPath = hasValidPathGetLast(fs, getArgs());
    if (latestInPath != nullptr && latestInPath->getIsDir()) {
        fs.setWorkingDirectory((Directory *) latestInPath);
    } else {
        cout << "The system cannot find the path specified" << endl;
    }

}

string CdCommand::toString() {
    return "cd";
}


LsCommand::LsCommand(string args) : BaseCommand(args) {

}

void LsCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    vector<string> argsDevidedBySpace = devideStringByChar(getArgs(), ' ');
    if (argsDevidedBySpace.size() == 1) {
        printLs(fs, true);
    } else if (argsDevidedBySpace.size() == 2) {
        if (argsDevidedBySpace.at(1) == "-s") {
            printLs(fs, false);
        } else {
            BaseFile *lastInPath = hasValidPathGetLast(fs, getArgs());
            if (lastInPath == nullptr || !lastInPath->getIsDir()) {
                cout << "The system cannot find the path specified" << endl;
            } else { //ls <path>
                Directory *p = ((Directory *) lastInPath);
                ((Directory *) lastInPath)->sortByName();
                string typeOfDoc;
                string nameOfDoc;
                int sizeOfDoc;

                size_t i;
                for (i = 0; i < p->getChildren().size(); i++) {
                    if (p->getChildren().at(i)->getIsDir()) {
                        typeOfDoc = "DIR";
                    } else {
                        typeOfDoc = "FILE";
                    }
                    nameOfDoc = p->getChildren().at(i)->getName();
                    sizeOfDoc = p->getChildren().at(i)->getSize();
                    cout << typeOfDoc << "\t" << nameOfDoc << "\t" << sizeOfDoc << endl;
                }

            }
        }

    } else if (argsDevidedBySpace.size() == 3) { //ls -s <path>
        BaseFile *lastInPath = hasValidPathGetLast(fs, "-s " + argsDevidedBySpace.at(2));
        if (lastInPath == nullptr || !lastInPath->getIsDir()) {
            cout << "The system cannot find the path specified" << endl;
        } else { //ls <path>
            Directory *p = ((Directory *) lastInPath);
            ((Directory *) lastInPath)->sortBySize();
            string typeOfDoc;
            string nameOfDoc;
            int sizeOfDoc;

            size_t i;
            for (i = 0; i < p->getChildren().size(); i++) {
                if (p->getChildren().at(i)->getIsDir()) {
                    typeOfDoc = "DIR";
                } else {
                    typeOfDoc = "FILE";
                }
                nameOfDoc = p->getChildren().at(i)->getName();
                sizeOfDoc = p->getChildren().at(i)->getSize();
                cout << typeOfDoc << "\t" << nameOfDoc << "\t" << sizeOfDoc << endl;
            }

        }
    }
}

string LsCommand::toString() {
    return "ls";
}

void LsCommand::printLs(FileSystem &fs, bool byName) {
    Directory *p = &fs.getWorkingDirectory();
    if (byName) {
        p->sortByName();
    } else {
        p->sortBySize();
    }
    string typeOfDoc;
    string nameOfDoc;
    int sizeOfDoc;

    size_t i = 0;
    for (; i < p->getChildren().size(); i++) {
        if (p->getChildren().at(i)->getIsDir()) {
            typeOfDoc = "DIR";
        } else {
            typeOfDoc = "FILE";
        }
        nameOfDoc = p->getChildren().at(i)->getName();
        sizeOfDoc = p->getChildren().at(i)->getSize();
        cout << typeOfDoc << "\t" << nameOfDoc << "\t" << sizeOfDoc << endl;
    }

}


MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {

}


void MkdirCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    Directory *preWorkDir = &fs.getWorkingDirectory();
    FileSystem &currFS = fs;
    vector<string> path = pathStringToVec(getArgs());
//    bool toContinue = true;
    string pathToCheck = "";
    size_t i;
    for (i = 0; i < path.size(); i++) {
        pathToCheck = pathToCheck + "/" + path.at(i);
        if (hasValidPathGetLast(currFS, pathToCheck) == nullptr) {
            break;

        } else {
            if (!currFS.getWorkingDirectory().getIsDir()) {
                cout << "The directory already exists" << endl;
                return;
            } else {
                currFS.setWorkingDirectory(dynamic_cast<Directory *> (hasValidPathGetLast(fs, pathToCheck)));
            }
        }
    }

    if (i == path.size()) {
        cout << "The directory already exists" << endl;
    } else {
        for (i; i < path.size(); i++) {
            if (path.at(i) == "/") {
                continue;
            }

            Directory *newDir = new Directory(path.at(i), &currFS.getWorkingDirectory());
            currFS.getWorkingDirectory().addFile(newDir);
            currFS.setWorkingDirectory(newDir);
        }
    }

    fs.setWorkingDirectory(preWorkDir);
}


string MkdirCommand::toString() {
    return "mkdir";
}


MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {}


void MkfileCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    BaseFile *lastDirPath;

    vector<string> brokenCmdBySpace = devideStringByChar(getArgs(), ' ');
    const string &path = brokenCmdBySpace.at(1);
    int sizeToAdd = stoi(brokenCmdBySpace.at(2));

    vector<string> pathByVec = pathStringToVec(path);
    string &nameOfFile = pathByVec.back();
    pathByVec.pop_back();

    if (pathByVec.empty()) {
        lastDirPath = &fs.getWorkingDirectory();
    } else {
        string pathStringUpToFather = genStringFromPathVector(pathByVec);
        lastDirPath = hasValidPathGetLast(fs, pathStringUpToFather); // father of file to add
    }

    if (lastDirPath != nullptr && lastDirPath->getIsDir()) {
        File *fileToAdd = new File(nameOfFile, sizeToAdd);
        if (!(dynamic_cast<Directory *> (lastDirPath))->getIfHasNameInChildrenByName(nameOfFile)) {

            (dynamic_cast<Directory *>(lastDirPath))->addFile(fileToAdd);

        } else {
            cout << "File already exist" << endl;
        }
    } else {
        cout << "The system cannot find the path specified" << endl;
    }
}

string MkfileCommand::toString() {
    return "mkfile";
}


CpCommand::CpCommand(string args) : BaseCommand(args) {

}

void CpCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }

    vector<string> devidedBySpace = devideStringByChar(getArgs(), ' ');

    string &source = devidedBySpace.at(1);
    string &destination = devidedBySpace.at(2);

    BaseFile *lastInSource = hasValidPathGetLast(fs, "dummy " + source);
    BaseFile *lastInDes = hasValidPathGetLast(fs, "dummy " + destination);


    if (lastInSource == nullptr || lastInDes == nullptr || !lastInDes->getIsDir()) {
        cout << "No such file or directory" << endl;

        // from here legal src and dest
    } else if ((dynamic_cast<Directory *> (lastInDes))->getIfHasNameInChildrenByName(lastInSource->getName())) {
//        cout << "The destination directory already contains a file/Directory called: " + lastInDes->getName() << endl;
        return;
    } else {
        if (!lastInSource->getIsDir()) {
            File *cpFile = new File(lastInSource->getName(), lastInSource->getSize());
            (dynamic_cast<Directory *> (lastInDes))->addFile(cpFile);
        } else {
            Directory *cpDir = new Directory(*(dynamic_cast<Directory *> (lastInSource)));
            (dynamic_cast<Directory *> (lastInDes))->addFile(cpDir);
            cpDir->setParent(dynamic_cast<Directory *> (lastInDes));
        }
    }
}

string CpCommand::toString() {
    return "cp";
}


MvCommand::MvCommand(string args) : BaseCommand(args) {
}

void MvCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    vector<string> devidedBySpace = devideStringByChar(getArgs(), ' ');

    string source = (string) devidedBySpace.at(1);
    string destination = (string) devidedBySpace.at(2);

    BaseFile *lastInSource = hasValidPathGetLast(fs, "dummy " + source);
    BaseFile *lastInDes = hasValidPathGetLast(fs, "dummy " + destination);
    Directory *srcFather;

    vector<string> pathFatherSrcByVec = pathStringToVec(devidedBySpace.at(1));
    pathFatherSrcByVec.pop_back();

    if (lastInSource && pathFatherSrcByVec.empty()) {
        srcFather = &fs.getWorkingDirectory();
    } else if (lastInSource) {
        string pathStringUpToFather = genStringFromPathVector(pathFatherSrcByVec);
        srcFather = dynamic_cast<Directory *>(hasValidPathGetLast(fs, pathStringUpToFather)); // father of file to add
    }

    if (lastInSource == nullptr || lastInDes == nullptr || !lastInDes->getIsDir()) {
        cout << "No such file or directory" << endl;
        return;
    } else if (((Directory *) lastInDes)->getIfHasNameInChildrenByName(lastInSource->getName())) {
//        cout << "The destination directory already contains a file/Directory called: " + lastInDes->getName() << endl;
        return;
    } else {
        if (!lastInDes->getIsDir()) {
            (dynamic_cast<Directory *>(lastInDes))->addFile(lastInSource);
            srcFather->removePointerOnly(lastInSource);

        } else {
            if (!isWorkingDirOrItsAncestor(fs, dynamic_cast<Directory *>(lastInSource))) {

                (dynamic_cast<Directory *>(lastInDes))->addFile(lastInSource);
                srcFather->removePointerOnly(lastInSource);
            }
        }
    }
}

string MvCommand::toString() {
    return "mv";
}


RenameCommand::RenameCommand(string args) : BaseCommand(args) {

}

void RenameCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    vector<string> devidedBySpace = devideStringByChar(getArgs(), ' ');
    string &newName = devidedBySpace.at(2);
    vector<string> vecPathToFile = pathStringToVec(devidedBySpace.at(1));
    // to get father:
    vecPathToFile.pop_back();
    Directory *parent;

    string pathFile = (string) devidedBySpace.at(1);
    BaseFile *lastInSource = hasValidPathGetLast(fs, "dummy " + pathFile);

    if (!lastInSource) {
        cout << "No such file or directory" << endl;
        return;
    } else if (lastInSource == &fs.getWorkingDirectory()) {
        cout << "Can’t rename the working directory" << endl;
        return;
    } else if (lastInSource == &fs.getRootDirectory()) {
        return;
    }

    if (!vecPathToFile.empty()) {

        string parentPathStr = genStringFromPathVector(vecPathToFile);
        BaseFile *parentP = hasValidPathGetLast(fs, "dummy " + parentPathStr);
        parent = dynamic_cast<Directory *>(parentP);
    } else {
        parent = &fs.getWorkingDirectory();
    }

    if (parent->getChildByName(newName)) {
        return;
    }

    lastInSource->setName(newName);


}

string RenameCommand::toString() {
    return "rename";
}


RmCommand::RmCommand(string args) : BaseCommand(args) {
}

void RmCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    vector<string> devidedBySpace = devideStringByChar(getArgs(), ' ');

    string source = (string) devidedBySpace.at(1);

    BaseFile *lastInSource = hasValidPathGetLast(fs, "dummy " + source);
    Directory *srcFather;

    vector<string> pathFatherSrcByVec = pathStringToVec(devidedBySpace.at(1));
    pathFatherSrcByVec.pop_back();

    if (lastInSource && pathFatherSrcByVec.empty()) {
        srcFather = &fs.getWorkingDirectory();
    } else if (lastInSource) {
        string pathStringUpToFather = genStringFromPathVector(pathFatherSrcByVec);
        srcFather = dynamic_cast<Directory *>(hasValidPathGetLast(fs, pathStringUpToFather)); // father of file to add
    }

    if (lastInSource == nullptr) {
        cout << "No such file or directory" << endl;
        return;

    } else {
        if (!lastInSource->getIsDir()) {
            srcFather->removeFile(lastInSource);

        } else {
            if (!isWorkingDirOrItsAncestor(fs, dynamic_cast<Directory *>(lastInSource))) {
                srcFather->removeFile(lastInSource);
            }
                // is working dir or some father
            else {
                cout << "Can't remove directory" << endl;
            }
        }
    }

}

string RmCommand::toString() {
    return "rm";
}


HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args),
                                                                                    history(history) {
}

void HistoryCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    for (size_t i = 0; i < history.size(); i++) {
        cout << i << "\t" << history.at(i)->getArgs() << endl;
    }
}

string HistoryCommand::toString() {
    return "history";
}


VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {

}

void VerboseCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    vector <string> vec = pathStringToVec(getArgs());
    int newVerbose = stoi(vec.at(0));
    if (newVerbose == 1|| newVerbose == 2 || newVerbose == 3){
        verbose = newVerbose;
    }
    else {
        cout << "Wrong verbose input" <<endl;
    }
}

string VerboseCommand::toString() {
    return "verbose";
}


ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {}

void ErrorCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    string wrongCmnd;
    if (getArgs().find(' ') != string::npos) {
        wrongCmnd = getArgs().substr(0, getArgs().find(' ') + 1);
    }
    else {
        wrongCmnd = getArgs();
    }

    cout << wrongCmnd + ": Unknown command" << endl;
}

string ErrorCommand::toString() {
    return "error";
}


ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem &fs) {
    if (verbose == 2 || verbose == 3){
        cout << getArgs() <<endl;
    }
    vector<string> argsV = devideStringByChar(getArgs(), ' ');
    string &numberOfCmnd = argsV.at(1);
    unsigned int realNumFromStr = (unsigned int)stoi(numberOfCmnd); //change string to number
    if (realNumFromStr < 0 || realNumFromStr > history.size()) {
        cout << "Command not found" << endl;
    } else {
        history.at(realNumFromStr)->execute(fs);
    }
}

string ExecCommand::toString() {
    return "exec";
}

