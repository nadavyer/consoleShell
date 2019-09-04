//
// Created by nadav on 09/11/17.
//
#include "../include/Files.h"
#include <vector>
#include <algorithm>

using namespace std;


BaseFile::BaseFile(string name) : name(name) {
}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName) {
    this->name = newName;
}

BaseFile::~BaseFile() {}


File::File(string name, int size) : BaseFile(name) {
    this->size = size;
}

int File::getSize() {
    return size;
}

const bool File::getIsDir() {
    return false;
}


Directory::Directory(string name, Directory *parent) : BaseFile(name), parent(parent), children() {
}


Directory *Directory::getParent() const {
    return parent;
}

void Directory::setParent(Directory *newParent) {
    parent = newParent;

}

void Directory::addFile(BaseFile *file) {
    bool hasFile = false;
    for (size_t i = 0; i < getChildren().size() && !hasFile; ++i) {
        if (getChildren().at(i)->getName() == file->getName())
            hasFile = true;
    }
    if (!hasFile) { children.push_back(file); }
}

void Directory::removeFile(string name) {
    for (size_t i= 0; i < children.size(); i++) {
        if (children.at(i)->getName() == name) {
            children.erase(children.begin() + i);
        }
    }

}

void Directory::removeFile(BaseFile *file) {
    for (size_t i = 0; i < children.size(); i++) {
        if (children.at(i) == file) {
            delete *(children.begin() +i);
            children.erase(children.begin() + i);
        }
    }

}

void Directory::sortByName() {
    sort(children.begin(), children.end(), comperatorByName);
}


void Directory::sortBySize() {
    sort(children.begin(), children.end(), comperatorBySize);
}

vector<BaseFile *> Directory::getChildren() const {
    return children;
}

int Directory::getSize() {
    int sum = 0;
    for (size_t i = 0; i < children.size(); i++) {
        sum = sum + children[i]->getSize();
    }
    return sum;
}

string Directory::getAbsolutePath() {
    Directory *parp = this->getParent();
    string pathS = this->getName();

    while (parp) {
        pathS = parp->getName() + "/" + pathS;
        parp = parp->getParent();
    }

    if (pathS.size() >= 2 && pathS.at(0) == '/' && pathS.at(1) == '/') {
        return pathS.substr(1);
    }

    return pathS;
}


bool Directory::comperatorByName(BaseFile *a, BaseFile *b) {
    bool ans;
    if (a->getName() < b->getName()) ans = true;
    else if (a->getName() > b->getName()) ans = false;
    else ans = comperatorBySize(a, b);
    return ans;

}

bool Directory::comperatorBySize(BaseFile *a, BaseFile *b) {
    bool ans;
    if (a->getSize() < b->getSize()) ans = true;
    else if (a->getSize() > b->getSize()) ans = false;
    else ans = comperatorByName(a, b);
    return ans;

}

//copy constructor
Directory::Directory(const Directory &other) : BaseFile(other.getName()), children(), parent(other.getParent()) {
    copy(other);
}

//copy assignment operator
Directory &Directory::operator=(const Directory &other) {
    clean();
    copy(other);

    return *this;
}

void Directory::copy(const Directory &other){
    parent = nullptr;
    for(BaseFile *child: other.children) {
        if (!child->getIsDir()) {
            string cpName = child->getName();
            int size = child->getSize();
            BaseFile *cpFile = new File(cpName, size);
            children.push_back(cpFile);
        } else {
            string cpName = child->getName();
            BaseFile *cpDir = new Directory(*dynamic_cast<Directory *>(child));
            dynamic_cast<Directory *>(cpDir)->setParent(this);
            children.push_back(cpDir);
        }
    }


    for (size_t  i = 0; i < other.getChildren().size(); i++) {
        if (!other.getChildren().at(i)->getIsDir()) {
            string cpName = other.getChildren().at(i)->getName();
            int size = other.getChildren().at(i)->getSize();
            File cpFile = File(cpName, size);
            children.push_back(&cpFile);
        } else {
            Directory(*(Directory *) other.getChildren().at(i));
        }

    }
}


//move constructor
Directory::Directory(Directory &&otherDir) : BaseFile(otherDir.getName()),
                                             children(move(otherDir.children) ), parent(otherDir.getParent()) {}

//move assignment operator
Directory Directory::operator=(Directory &&otherDir) {
    parent = otherDir.getParent();
    otherDir.setParent(nullptr);
    children = move(otherDir.children);

    return *this;
}

const bool Directory::getIsDir() {
    return true;
}

bool Directory::getIfHasNameInChildrenByName(string name) {
    return getChildByName(name) != nullptr;
}

BaseFile *Directory::getChildByName(string &name) {
    for (size_t i = 0; i < children.size(); ++i) {
        if (children.at(i)->getName() == name) {
            return children.at(i);
        }
    }
    return nullptr;
}

int Directory::indxOfChild(BaseFile *child) {
    for (size_t  i = 0; i < children.size(); ++i) {
        if (children.at(i)->getName() == child->getName())
            return i;
    }
}

void Directory::clean() {
    for (size_t i = 0; i < getChildren().size(); ++i) {
        if (children.at(i)->getIsDir()) {
            (*(Directory *) children.at(i)).setParent(nullptr);
            delete getChildren().at(i);
        } else {
            removeFile(children.at(i));
        }
    }
}

//destructor
Directory::~Directory() {
    clean();
}

void Directory::removePointerOnly(BaseFile *toRm) {
    vector<BaseFile *>::iterator rm_iter = find(children.begin(), children.end(), toRm);
    if (rm_iter != children.end()) {
        children.erase(rm_iter);
    }

}




