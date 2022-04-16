#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(string FileName, FsFile* fsi) {
    this->file_name = FileName;
    this->fs_file = fsi;
    this->inUse = true;
}

string FileDescriptor::getFileName() {
    return file_name;
}
FsFile* FileDescriptor::getFs_file() {
    return fs_file;
}
bool FileDescriptor::getInUse() {
    return inUse;
}
void FileDescriptor:: setInUse(bool y){
    this->inUse = y;
}

