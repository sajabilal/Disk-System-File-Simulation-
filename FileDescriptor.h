#ifndef UNTITLED2_FILEDESCRIPTOR_H
#define UNTITLED2_FILEDESCRIPTOR_H
#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cassert>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <bits/stdc++.h>
#include "FsFile.h"
using namespace std;

class FileDescriptor {
    string file_name;
    FsFile* fs_file;
    bool inUse;

public:
    FileDescriptor(string FileName, FsFile* fsi);
    string getFileName();
    FsFile* getFs_file();
    bool getInUse();
    void setInUse(bool y);
};

#endif //UNTITLED2_FILEDESCRIPTOR_H