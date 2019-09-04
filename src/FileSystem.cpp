//
// Created by nadav on 09/11/17.
//

#include "../include/FileSystem.h"


FileSystem::FileSystem() : rootDirectory(new Directory("/", nullptr)), workingDirectory(rootDirectory) {}

Directory &FileSystem::getRootDirectory() const {
    Directory &ref1 = *rootDirectory;
    return ref1;
}

Directory &FileSystem::getWorkingDirectory() const {
    Directory &ref2 = *workingDirectory;
    return ref2;
}

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}

//copy constructor
FileSystem::FileSystem(const FileSystem &other) : workingDirectory(), rootDirectory(){
    copy(other);
}

//FileSystem::FileSystem(FileSystem &&steal) { //TODO: do!
//
//}

//move assignment operator
FileSystem &FileSystem::operator=(FileSystem &&other) {
    if (this != &other) {
        clean();
        rootDirectory = other.rootDirectory;   //TODO: zligat zikaron
        workingDirectory = other.workingDirectory;
    }
    return *this;
}

//destructor
FileSystem::~FileSystem() {
    clean();
}

//copy assignment operator
FileSystem &FileSystem::operator=(const FileSystem &other) {
    if (this != &other) {
        clean();
        copy(other);
    }

    return *this;
}

void FileSystem::clean() {
    delete rootDirectory;
    workingDirectory = nullptr;
    rootDirectory = nullptr;
}

Directory *FileSystem::getNewWD(Directory *otherWD, Directory &newRoot) {
    string path = "";
    Directory *curr = otherWD;
    Directory *myWd = &newRoot;
    if (otherWD->getAbsolutePath() == "/") {
        path = "/";
    }
    else {
        while (curr && curr->getParent() != nullptr) {
            path = "/" + curr->getName() + path;
            curr = curr->getParent();
        }
    }

    vector<string> pathInVec = pathStringToVec(path);

    for (size_t i = 0; i < pathInVec.size(); ++i) {
        if (pathInVec.at(i) == "/"){
            continue;
        }
        myWd = dynamic_cast<Directory *>(myWd->getChildByName(pathInVec.at(i)));

    }

    return myWd;

}

vector<string> FileSystem::pathStringToVec(string givenPath) {
    vector<string> path;
    string str = givenPath;
    if (str.at(0) == '/') {
        path.push_back("/");
    }
    string word = "";
    if (givenPath != "/") {
        while (str != "") {
            int i;
            for (i = 0; str.at(i) != '/'; i++) {
                word = word + str.at(i);
            }
            str = str.substr(i + 1);
            path.push_back(word);
            word = "";
        }
    }

    return path;
}

void FileSystem::copy(const FileSystem &other) {
    const Directory &otherRoot = other.getRootDirectory();
    rootDirectory = new Directory(otherRoot);
    workingDirectory = getNewWD(other.workingDirectory, *rootDirectory);

}




