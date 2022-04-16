#include"fsDisk.h"
#include"FileDescriptor.h"

fsDisk::fsDisk() {

    sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
    assert(sim_disk_fd);
    for (int i=0; i < DISK_SIZE ; i++) {
        int ret_val = fseek ( sim_disk_fd , i , SEEK_SET );
        ret_val = fwrite( "\0" ,  1 , 1, sim_disk_fd);
        assert(ret_val == 1);
    }
    fflush(sim_disk_fd);
    this->block_size = 0;
    this->is_formatted = false;
    this->BitVectorSize = DISK_SIZE ;
    this->BitVector = (int*) malloc (sizeof(int)*BitVectorSize);
    updateBitVector();
}

void fsDisk:: listAll() {
    node* mainDir2= NULL;
    mainDir2 = mainDir;
    int i ;
    for (i=0; mainDir2 !=mainDir1; i++ ) {

        cout << "index: " << i << ": FileName: " << mainDir2->value<<  " , isInUse: " << FD[i].getInUse() <<  endl;

        mainDir2++;

    }
    char bufy;
    cout << "Disk content: '" ;
    for (i=0; i < DISK_SIZE ; i++) {
       fseek ( sim_disk_fd , i , SEEK_SET );
       fread(  &bufy , 1 , 1, sim_disk_fd );
        cout << bufy;
    }
    cout << "'" << endl;
}
 void fsDisk:: fsFormat( int blockSize ) {
    this->block_size=blockSize;
    this->is_formatted = true;
    this->freeBlocks = DISK_SIZE/blockSize;
     }
int fsDisk::CreateFile(string fileName) {
    if(is_formatted==false){//checks if disk formatted
        printf("disk is not formatted\n");
        return -1;
    }
    if (sim_disk_fd!= nullptr && mainDir1 - mainDir!= DISK_SIZE){//checks if disk is started
        node* mainDir2 = mainDir;
        mainDir1++;
        while(  mainDir2 != mainDir1) {

            if (fileName == mainDir2->value) {//checks if file exists
                printf("file is created already \n");
                return -1;
            }
            mainDir2++;
        }
        mainDir1--;
        //if file doesnt exist it creates and opens it
        int newFile_fd  = open(fileName.c_str() ,O_RDWR| O_CREAT | O_TRUNC, 0644);//creates and Opens
        if(newFile_fd == -1) { // file couldn't be opened
            perror("unable to open or create the file\n");
            exit(1);
        }
        FsFile *fs = new  FsFile(block_size);//creating file Descriptor object
        FsFile* fsi ;
        fsi = fs;
        updateOpenFileDescriptors(FD, fileName, fsi);
        updateMainDir(fileName, *fs);
        return newFile_fd-4;
    }
    printf("disk not started or doesnt have enough space\n");
    return -1;
}
FileDescriptor* fsDisk::updateOpenFileDescriptors(vector<FileDescriptor> &FD, string fileName, FsFile* fs) {//updates openFileDescriptors in case of opening a file
    FileDescriptor* fdObject = new FileDescriptor(fileName, fs);
    FD.push_back(*fdObject);
    return fdObject;
}
void fsDisk::updateMainDir(string fileName, FsFile fs){//updates mainDir after creation of file
    node n1 = {fileName ,fs};
    *mainDir1 = n1;
    mainDir1++;
}
int fsDisk::OpenFile(string fileName) {
    if(is_formatted==false){//checks if the disk is formatted
        printf("disk is not formatted\n");
        return -1;
    }
    if (sim_disk_fd != nullptr) {//checks if disk started
        node *mainDir2 = mainDir;
        mainDir1++;
        while (mainDir2 != mainDir1) {//checks if file created
            if (fileName == mainDir2->value) {//checks if the file exists if file exists
                int fd;
                for (int i = 0; i < FD.size(); ++i) {//checks if the file is open already6
                    if (FD[i].getFileName() == fileName && FD[i].getInUse() == 1) {
                        printf("file is open already \n");
                        fd = i + 4;
                        mainDir1--;
                        return fd-4;
                    }
                    else if (FD[i].getFileName() == fileName && FD[i].getInUse() == 0){
                        printf("file will be opened \n");
                        FD[i].setInUse(1);
                        fd = i + 4;
                        mainDir1--;
                        return fd-4;
                    }
                }
            }
            mainDir2++;
        }
        mainDir1--;
    }
    printf("no such file \n");
    return -1;
}
string fsDisk::CloseFile(int fd) {
    if (is_formatted == false) {//checks if disk is formatted
        return "disk is not formatted\n";
    }
    if (sim_disk_fd != nullptr) {//checks if disk started
        for (int i = 0; i < FD.size(); ++i) {
            if (FD[i].getFileName() == FD[fd].getFileName()) {//checks if file is open
                printf("file will be closed \n");
//                close(i + 4);
                FD[fd].setInUse(0);
                return FD[fd].getFileName();
            }
        }
    }
    printf("no such file \n");
    return "-1";
}
void fsDisk::addRestBlock(int fd, char* buf){//calculate location of the place where the buf needed to be added to the rest of the block and passes to update properties
    int fileSize = FD[fd-4].getFs_file()->getfile_size();
    int indexBlock = FD[fd-4].getFs_file()->getIndex_block();
    int block_in_use = FD[fd-4].getFs_file()->getBlock_in_use();
    fseek(sim_disk_fd,indexBlock*block_size,SEEK_SET);
    char index[block_size];
    fread(index, 1, block_size, sim_disk_fd) ;
    int index1 = binaryToDecimal(index[block_in_use-1]);
    updateProperties((index1*block_size)+(fileSize%block_size),fd,buf,strlen(buf));
}
int fsDisk:: FDedit(int fd){//offset of FD
    fd = fd+4;
    return fd;
}
int fsDisk::WriteToFile(int fd, char *buf, int str_buf) {
    fd = FDedit(fd);
c=0;
        if(!is_formatted){//checks if disk is formatted
            printf("disk is not formatted\n");
            return -1;
        }
        if (sim_disk_fd != nullptr && fd<=FD.size()+4 && freeBlocks!= 0){//checked if the disk has started and the file is open and there is space in the disk
            if (FD[fd-4].getInUse() == 0) {//checks if file is open
                printf("file is not open\n");
                return -1;
            }
            int fileSize = findFD(fd, FD);//size of the file the input fd points to, if it is maximum it will drop
            if(fileSize ==-2 ){
                return -1;
            }
            else{
                buf = checkSpace(fd,str_buf, buf);//how much of the string may be saved in the file, returns the part of buf that may be saved
                if(buf == nullptr) return -1;
                int rest = fileSize%block_size;//space left in the last block used by file
                if (rest == 0) {// the used blocks are full
                    addStringToFile(fd, buf, str_buf, FD);
                    return 1;
                }
                else {// used blocks are not full (the last one is not full

                           ///1- write to fill the last block
                           if (strlen(buf) <= rest) {//the buf to insert less that the rest of the space of the last block
                               addRestBlock(fd,buf);
                               return 1;
                           }
                           else if (strlen(buf) > rest) {//the buf to insert bigger than the rest of the space of the last block
                               write(fd, (const void *) buf, (size_t) rest);
                               char buf1[rest];
                               char buf2[strlen(buf) - rest];
                               int y;
                               for (y = 0; y < rest; y++)
                                   buf1[y] = buf[y];
                               buf1[rest]='\0';
                               for (int x = 0; x < strlen(buf) - rest; x++){
                                   buf2[x] = buf[y];
                                   y++;
                                }
                               buf2[strlen(buf) - rest]='\0';
                               addRestBlock(fd,buf1);//add the allowed ammount to the rest of the block
                               addStringToFile (fd, buf2,  strlen(buf2),  FD);//write to the rest to new needed blocks
                               return 1;
                           }
                       }
                   }
        }
        printf("it is either the disk is not started, FD is not opened or the disk is full\n");
        return -1;
    }
char* fsDisk:: checkSpace (int fd ,int str_buf, char *buf){//returns how much space  left in file
    int fileSize = FD[fd-4].getFs_file()->getfile_size();
    int availableSpace = block_size*block_size-fileSize;
        if (availableSpace >=  str_buf )
            return buf;
        else if( availableSpace == 0 ){
            printf("the file is full  \n");
            return nullptr;
        }
        else {
            buf[availableSpace] = '\0';
            printf("since there is no enough space not the whole string has been saved, the following part only: %s \n", buf);
            return buf;
        }
}
int fsDisk::findFD (int fd, vector<FileDescriptor> &FD) {//checks if the file got to the maximum size
        for (int i = 0; i < FD.size(); ++i) {//find the FD needed to get FSFile
            if (i == fd-4) {//file is open , found in FD
                string fileName = FD[i].getFileName();
               int fileSize = FD[i].getFs_file()->getfile_size();
                if(fileSize > block_size*block_size){
                    printf("the file has the maximum size, unable to write to file \n ");
                    return-2;
                }
                return fileSize;
            }
        }
        printf("file is not open FindFD\n");
        return -2;
}
int fsDisk :: newIndexBlock (int fd){//find indexblock for new files
    int indexBlock;
    for (int i = 0; i < BitVectorSize ; ++i) {
        if (BitVector[i] == 0){ //block number i  is free
            FD[fd-4].getFs_file()->setIndex_block(i);
            printf("fd num %d index block set to %d\n", fd, i);
            indexBlock=i;
            BitVector[indexBlock] = 1;
            return indexBlock;
        }
    }
    printf("no available  blocks to allocate index block for fd %d\n",fd);
    return -1;
}
int fsDisk :: neededIndexes (int str_buf){//checks number of blocks needed for rach entery
    int needed;
    if (str_buf>=block_size) {
        if(str_buf%block_size ==0)
            needed = (str_buf / block_size);
        else
            needed = (str_buf / block_size) + 1; //needed space for the buf
    }
    else
        needed = 1;
    return needed;
}
void fsDisk :: updateProperties  (int i,int fd, char* buf,int str_buf){
    int fileSize = FD[fd-4].getFs_file()->getfile_size();
    int blockInUse = (fileSize+str_buf)/block_size;
    if((fileSize+str_buf)%block_size != 0) blockInUse = blockInUse+1;
    write(fd,(const void*) buf, block_size);//writes to file
    fseek(sim_disk_fd,i,SEEK_SET);//write to hard disk
    fwrite(buf, sizeof (char), strlen(buf), sim_disk_fd);
    ///3- update FsFile
    FD[fd-4].getFs_file()->setFile_size((fileSize+strlen(buf)));//update filesize
    FD[fd-4].getFs_file()->setBlock_in_use(blockInUse);//update block in use
    ///5- update free blocks of hard disk
    freeBlocks= freeBlocks-FD[fd-4].getFs_file()->block_in_use;//update free blocks
    printf("wrote %ld chars into file fd #%d\n",strlen(buf),fd-4);
}
void fsDisk :: addStringToFile (int fd, char *buf, int str_buf, vector<FileDescriptor> &FD){
    c=0;
    int indexBlock =FD[fd-4].getFs_file()->getIndex_block();//find index block
    if(indexBlock ==-1){//if index block is -1 we will find one
        indexBlock= newIndexBlock(fd);
        if(indexBlock ==-1) exit (-1);
    }
    int needed = neededIndexes(str_buf);//how many blocks needed to save the data
    int j = 0;
    for (int i = 0; i < BitVectorSize && j<needed; ++i) {

        if (BitVector[i] == 0 ) {//block number i  is free & reserve it
            printf("block number %d is free \n", i);
            c=0;
            c = decToBinary(i, c);//change DEc to Symbol
            char str[block_size] ;//to add all the content of index block
            size_t ffseek = fseek(sim_disk_fd,indexBlock*block_size,SEEK_SET);//set to index block for reading
            if(ffseek==-1){
                perror("unable to fseek indexbloxk from hardDisk for reading\n");
                break;
            }
            else {
                size_t ffread= fread(str, 1, block_size, sim_disk_fd) ;//read content of index block for reading
                if (ffread == -1) {
                    perror("unable to read indexblock from hardDisk\n");
                    break;
                } else {
                    for(int k=0; k<block_size; k++) {//adding c to str
                        if (str[k] == '\0'){
                            str[k] = c;
                            break;
                        }
                    }
                    size_t fffseek = fseek(sim_disk_fd, indexBlock * block_size , SEEK_SET);//set to index block for writing
                    if (fffseek == -1) {
                        perror("unable to fseek indexblock from hardDisk for writing\n");
                        break;
                    } else {
                        size_t fffwrite =fwrite(str, sizeof(char), block_size, sim_disk_fd);//read content of index block for writing
                        if (fffwrite == -1) {
                            perror("unable to write indexblock from hardDisk\n");
                            break;
                        } else {
                            BitVector[i] = 1;//reserve the bit
                            char buf1[block_size];
                            int f=0;
                            for(int r=(j*block_size);r<strlen(buf);r++ ) {
                                buf1[f] = buf[r];
                                f++;
                            }
                            buf1[block_size] = '\0';//the part of Buf which has been written to the harddisk
                            j++;
                            updateProperties(i*block_size,fd,buf1,block_size);
                        }
                    }
                }
            }
        }
    }
    if(j<needed-1) {//2- no enough free blocks
        printf("no enough free blocks not all the data added\n");
    }

}
void fsDisk :: updateBitVector( ) {
        for(int i=0; i<BitVectorSize;i++){//updating bitVector before starting
            BitVector[i] = 0;
        }
}
char fsDisk ::decToBinary(int n , char &c){
    int binaryNum[8];// counter for binary array
    int i = 0;
    while (n > 0) {// storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    for (int j = i - 1; j >= 0; j--) {// printing binary array in reverse order
        if (binaryNum[j]==1){
            c = c | 1u << j;
            }
    }
    return c;
}
string fsDisk ::ReadFromFile(int fd, char *buf, int len ){
    if(!is_formatted){
        printf("disk is not formatted\n");
        return "-1";
    }
    if (sim_disk_fd != nullptr && fd<=FD.size()) {
        int blockToRead = (len / block_size) + (len%block_size);//how many blocks to read
        char blocks[blockToRead];//array of Symbols in index block
        int indexBlock = FD[fd].getFs_file()->getIndex_block();
        fseek(sim_disk_fd,indexBlock*block_size,SEEK_SET);//situate on index block
        fread(blocks, 1, block_size, sim_disk_fd) ;//reading symbols inside index block
        int blocks1 [block_size];
        for (int x =0; x<block_size; x++){//change symbols to number of blocks
            blocks1[x] = binaryToDecimal(blocks[x]);
        }
        int y=0;
        for (int k =0 ; k<blockToRead; k++){
            char block[block_size];//what each symbol points to
            fseek(sim_disk_fd,blocks1[k]*block_size,SEEK_SET);//situate on block
            fread(block, 1, block_size, sim_disk_fd) ;//reading data
            for(int u =0; u<block_size ; u++){//fill in buf
                buf[y] =block[u];
                y++;
                if(u+1 ==block_size) buf[y]='\0';
            }
        }
        return buf;
    }
    else{
        printf("it is either the disk is not started or FD is not opened \n");
        return "-1";
    }
}
int fsDisk ::binaryToDecimal(char &b){
    int n = 0;
    for(int i = 0; i<8; i++)
    n=n+(1u&(b>>i))*pow(2,i);
    return n;
}
int fsDisk ::DelFile( string FileName ) {
    if(is_formatted==false){//checks if the disk is formatted
        printf("disk is not formatted\n");
        return -1;
    }
    if (sim_disk_fd != nullptr) {//checks if disk started
        node *mainDir2 = NULL;
        mainDir2= mainDir;
        while (mainDir2 != mainDir1) {//loops over MainDir to check if the file created
            if (FileName == mainDir2->value) {//checks if the file exists if file exists to delete it
                for (int i = 0; i < FD.size(); ++i) {//checks if the file is open already
                    if (FD[i].getFileName() == FileName) {//if found in FD, then deleting
                        printf("file is being deleted from OpenFileDescriptor Table \n");
                        remove((FileName).c_str());
                        int len = FD[i].getFs_file()->getfile_size();
                        int blockToRead = (len / block_size) + (len%block_size);//how many blocks to read
                        char blocks[blockToRead];//array of Symbols in index block
                        int indexBlock = FD[i].getFs_file()->getIndex_block();
                        fseek(sim_disk_fd,indexBlock*block_size,SEEK_SET);//situate on index block
                        fread(blocks, 1, block_size, sim_disk_fd) ;//reading symbols inside index block
                        int blocks1 [block_size];
                        for (int x =0; x<block_size; x++){//change symbols to number of blocks
                            blocks1[x] = binaryToDecimal(blocks[x]);
                        }
                        for (int k =0 ; k<blockToRead; k++){
                            fseek(sim_disk_fd,blocks1[k]*block_size,SEEK_SET);//situate on block
                            fwrite("\0", 1, block_size, sim_disk_fd) ;//replace data with null
                        }
                        fseek(sim_disk_fd,indexBlock*block_size,SEEK_SET);//situate on index block
                        fwrite("\0", 1, block_size, sim_disk_fd) ;//reading symbols inside index block
                        BitVector[indexBlock]=0;
                        BitVector[i]=0;
                        FD.erase(FD.begin()+i);
                        freeBlocks++;
                        printf("file is being deleted from MainDir Table \n");
                        mainDir2->value = "-1";
                        mainDir2->next= '\0';
                        mainDir2 = nullptr;
                        delete(mainDir2);
                        return i;
                    }
                }
            }
            mainDir2++;
        }
        printf("file doesnt exist \n");
        return -1;
    }
    printf("disk is not started \n");
    return -1;
}
