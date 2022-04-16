# Disk-System-File-Simulation-

==Description==

==Description of Disk-System-File-Simulation.c==
this code is a simulation of the disk file system, it performes the harddisk tasks such as Read, Write and creating file. 
we use a file to save on as a hard disk, where we save file data location pointing to the data itself. 
location of data and location blocks(index blocks" are located by a vector which indecate which bits of the hards disk is free and may be used.
through index block all data manipulationmay take place(writing, deleting, reading, etc) 

==functions== 
fsDisk Class 

    fsDisk();//constructor initiates variables and starts the disk
    void listAll();//prints disk content
    void fsFormat( int blockSize  );//formats the disk before starting
    int CreateFile(string fileName);//creates files
    FileDescriptor* updateOpenFileDescriptors(vector<FileDescriptor> &FD, string fileName, FsFile* fs) ;//updates FD about open file
    void updateMainDir(string fileName, FsFile fs);//updates MainDir about created files
    int OpenFile(string fileName);//open closed created file
    string CloseFile(int fd) ;//closes open files
    int WriteToFile(int fd, char *buf,int str_buf) ;//writes to open files
    int neededIndexes (int str_buf);//checks how many blocks needed to save given buffer
    int  newIndexBlock (int fd);//determines an index block for new files
    void updateBitVector();//initiates BitVector
    void addRestBlock(int fd,char* buf);//adds buffer to the available space in block (rest of it)
    void addStringToFile (int fd, char* buf, int str_buf, vector<FileDescriptor> &FD);//used by WriteFile, responsible for checking symbols in index block and determine suitable location
    char* checkSpace (int fd, int str_buf, char *buf);//checks how much available space for writing is left in the file
    void  updateProperties  (int position,int fd, char *buf,int str_buf);//updates file, hardware and all their properties such as file size and free blocks    
    int findFD (int fd, vector<FileDescriptor> &FD);//provides the filesize if it didnt get to maximum size
    char decToBinary(int n ,char &c);//used to transform indexes from decimal numbers to Anscii symbols
    int DelFile( string FileName ) ;//deletes file from hard disk
    string ReadFromFile(int fd, char *buf, int len );//reads file
    int binaryToDecimal(char &n);//transforms from chars into numbers
    static int  FDedit(int fd);//FD offset

==files used== 
    DISK_SIM_FILE.txt

==how to compile== 
 g++ -o Disk-System-File-Simulation.2021  main.cpp fsDisk.cpp FsFile.cpp FileDescriptor.cpp
 to run: ./Disk-System-File-Simulation.2021

==input==
start with formating the disk to the needed block size by inserting the size to case #2 

to print choose 1

to create a file choose 3 with input (filename)

to open choose 4 and as input add filename

to close choose 5 and ass file descriptor number as an input 

to write choose 6 and add file descriptor number and then the string to write as input 

to read choose 7 with file descriptor number and the length o the needd string to be read as an input 

to delete a file choose 8 and add file name as an input 

==output=== 
depending on the chosen option 
1-> prints data in gard disk 
2-> no output 
3-> name and file descriptor number of the created file 
4-> name and file descriptor number of the opened file 
5-> name and file descriptor number of the closed file
6-> number of chars written into file with it's file descriptor 
7->the requested string to be read
8-> name and file descriptor number of the deleted file 
