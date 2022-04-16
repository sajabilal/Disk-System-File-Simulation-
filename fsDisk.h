#ifndef UNTITLED2_FSDISK_H
#define UNTITLED2_FSDISK_H


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
#include <bits/stdc++.h>
#include <stdlib.h>
#include <vector>
#include "FsFile.h"
#include  "FileDescriptor.h"
using namespace std;
#define DISK_SIZE 256
#define DISK_SIM_FILE "DISK_SIM_FILE.txt"



class fsDisk {
    FILE *sim_disk_fd;
    bool is_formatted; // is the disk formatted or not
    int BitVectorSize;
    int *BitVector;// is bit in i location free or not
    // BitVector - "bit" (int) vector, indicate which block in the disk is free
//              or not.  (i.e. if BitVector[0] == 1 , means that the
//             first block is occupied.
    struct node {//each node contains file name and fs of file
        string value; //this is filename
        FsFile next; //this is the node the current node points to. this is how the nodes link
//        FileDescriptor* fd; //fileDescriptor related
    };
    node* mainDir= (node*) malloc ((DISK_SIZE/2)*sizeof(node));//array of allocated nodes (maximum number of files_
    vector<FileDescriptor> FD;
    node* mainDir1 = mainDir;
    int block_size;
    int maxSize;//maximum block number for a file equal to block_size^2
    int freeBlocks ; //number of free blocks
    char c;
public:
    fsDisk();//constructor initiates variables and starts the disk
    void listAll();//prints disk content
    void fsFormat( int blockSize  );//formats the disk before starting
    int CreateFile(string fileName);//creates files
    FileDescriptor* updateOpenFileDescriptors(vector<FileDescriptor> &FD, string fileName, FsFile* fs) ;//updates FD about open file
    void updateMainDir(string fileName, FsFile fs);//updates MainDir about created files
    int OpenFile(string fileName);//open closed created file
    string CloseFile(int fd) ;//closes open files
    int WriteToFile(int fd, char *buf,int str_buf) ;//writes to open files
    void updateBitVector();//initiates BitVector
    void addStringToFile (int fd, char* buf, int str_buf, vector<FileDescriptor> &FD);//used by WriteFile, responsible for checking symbols in index block and determine suitable location
    char* checkSpace (int fd, int str_buf, char *buf);//checks how much available space for writing is left in the file
    int findFD (int fd, vector<FileDescriptor> &FD);//provides the filesize if it didnt get to maximum size
    char decToBinary(int n ,char &c);//used to transform indexes from decimal numbers to Anscii symbols
    int DelFile( string FileName ) ;//deletes file from hard disk
    string ReadFromFile(int fd, char *buf, int len );//reads file
    int  newIndexBlock (int fd);//determines an index block for new files
    int neededIndexes (int str_buf);//checks how many blocks needed to save given buffer
    void  updateProperties  (int position,int fd, char *buf,int str_buf);//updates file, hardware and all their properties such as file size and free blocks
    int binaryToDecimal(char &n);//transforms from chars into numbers
    void addRestBlock(int fd,char* buf);//adds buffer to the available space in block (rest of it)
    static int  FDedit(int fd);//FD offset
};


#endif //UNTITLED2_FSDISK_H
