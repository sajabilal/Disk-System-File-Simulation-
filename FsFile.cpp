#include "FsFile.h"

FsFile::FsFile(int _block_size) {
    this->file_size = 0;
    this->block_in_use = 0;
    this->block_size = _block_size;
    this->index_block = -1;
}

int FsFile:: getfile_size(){
    return this->file_size;
}

void  FsFile:: setFile_size(int fileSize){
    this->file_size = fileSize;
}
void  FsFile:: setBlock_size(int blockSize){
    this->block_size = blockSize;
}
void  FsFile:: setBlock_in_use(int blockInUse){
   this-> block_in_use = blockInUse;
}
void FsFile:: setIndex_block(int indexBlock){
    this->index_block = indexBlock;
}
int  FsFile::getIndex_block(){
    return this->index_block;
}
int FsFile::getBlock_in_use(){
    return this->block_in_use;
}
