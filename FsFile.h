

#ifndef UNTITLED2_FSFILE_H
#define UNTITLED2_FSFILE_H

class FsFile {

    int file_size;
    int block_in_use;
    int index_block;
    int block_size;

public:

    FsFile(int _block_size) ;
    void setFile_size(int fileSize);
    void setBlock_size(int blockSize);
    void setBlock_in_use(int blockInUse);
    void setIndex_block(int indexBlock);
    int getIndex_block();
    int getfile_size();
    friend class fsDisk;
    int getBlock_in_use();
};


#endif //UNTITLED2_FSFILE_H
