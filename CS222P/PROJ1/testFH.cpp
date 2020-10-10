/*
Author: Yuting Xie
Date:2020-10-10
Description: 222P proj1 self test
*/ 

#include <cstdio>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <cstring>

typedef int RC ;
typedef unsigned PageNum ;

#define DEFAULT_FILENAME "default.dat"
#define LOGERR(a) printf("ERROR: \t"); printf(a); printf("\n");
#define LOGINFO(a) printf("INFO: \t"); printf(a); printf("\n");
#define PAGE_SIZE 4096


class FileHandle;

class PagedFileManager {
public:
    static PagedFileManager &instance();                                // Access to the singleton instance

    RC createFile(const std::string &fileName);                         // Create a new file
    RC destroyFile(const std::string &fileName);                        // Destroy a file
    RC openFile(const std::string &fileName, FileHandle &fileHandle);   // Open a file
    RC closeFile(FileHandle &fileHandle);                               // Close a file

protected:
    PagedFileManager();                                                 // Prevent construction
    ~PagedFileManager();                                                // Prevent unwanted destruction
    PagedFileManager(const PagedFileManager &);                         // Prevent construction by copying
    PagedFileManager &operator=(const PagedFileManager &);              // Prevent assignment

private:
    std::unordered_map<std::string, unsigned> nameToHandleId;
    std::unordered_map<unsigned,FileHandle*> idToHandle;
};

class FileHandle {
public:
    // variables to keep the counter for each operation
    unsigned readPageCounter;
    unsigned writePageCounter;
    unsigned appendPageCounter;

    FileHandle();                                                       // Default constructor
    ~FileHandle();                                                      // Destructor

    //show info
    void printInf();
    //for mgr to set fp
    RC setFp(FILE* fp);
    //for mgr to acquire fp
    FILE* getFp();
    RC readPage(PageNum pageNum, void *data);                           // Get a specific page
    RC writePage(PageNum pageNum, const void *data);                    // Write a specific page
    RC appendPage(const void *data);                                    // Append a specific page
    unsigned getNumberOfPages();                                        // Get the number of pages in the file
    RC collectCounterValues(unsigned &readPageCount, unsigned &writePageCount,
                            unsigned &appendPageCount);                 // Put current counter values into variables

private:

    //Meta data
    //Physical file
    FILE* fp;
    //The unique id identifies a unique file handle instance
    unsigned int uid;
    static unsigned int idGen;
    //Number of page
    PageNum numPage;

    //calculate the exact offset from the start of the fiel for a apecific pageNum
    unsigned getOffsetByPageNum(PageNum pageNum);
    //get total length of file
    unsigned getFileSize();
    //update the 4 metadata ahead of the file, do this for every operation
    RC updateMetaDataToFile();
    //retreive the 4 metadata ahead of the file, do this when open an existing file
    RC updateMetaDataFromFile();



};

PagedFileManager &PagedFileManager::instance() {
    static PagedFileManager _pf_manager = PagedFileManager();
    return _pf_manager;
}

PagedFileManager::PagedFileManager() = default;

PagedFileManager::~PagedFileManager() = default;

PagedFileManager::PagedFileManager(const PagedFileManager &) = default;

PagedFileManager &PagedFileManager::operator=(const PagedFileManager &) = default;

RC PagedFileManager::createFile(const std::string &fileName) {
    //check if file exist
    if (0 == access(fileName.c_str(),F_OK)){
        LOGERR("file already exist failed to create file")
        return -1;
    }

    FILE* fp = fopen(fileName.c_str(),"wb");
    if (!fp){
        LOGERR("unable to open file in createFile")
        return -1;
    }

    fclose(fp);

    return 0;
}

RC PagedFileManager::destroyFile(const std::string &fileName) {
    //check if file exist
    if (0 != access(fileName.c_str(),F_OK)){
        LOGERR("file not exists, no need to destroy")
        return -1;
    }

    // todo: check if there are FH attached to this file
    // auto nameToId = nameToHandleId.find(fileName);
    // if (nameToId!=nameToHandleId.end()){

    //     auto idToFh = idToHandle.find(nameToId->second);
    //     closeFile(*(idToFh->second));

    //     //clean
    //     nameToHandleId.erase(nameToId);
    //     idToHandle.erase(idToFh);
    // }

    if (0 != remove(fileName.c_str())){
        LOGERR("failed to remove file in destroyFile")
        return -1;
    }

    return 0;
}

RC PagedFileManager::openFile(const std::string &fileName, FileHandle &fileHandle) {
    if (0 != access(fileName.c_str(),F_OK)){
        LOGERR("file not exist failed in openFile")
    }

    // read, write, binary
    FILE* fp = fopen(fileName.c_str(),"r+b");
    if(!fp){
        LOGERR("unable to open file in openFile")
        return -1;
    }

    return fileHandle.setFp(fp);

}

RC PagedFileManager::closeFile(FileHandle &fileHandle) {
    FILE* fp = fileHandle.getFp();
    if(!fp){
        LOGERR("already closed in closeFile")
        return -1;
    }

    fclose(fp);
    fileHandle.setFp(nullptr);

    return 0;
}


//FileHandle below
unsigned int FileHandle::idGen = 0;

FileHandle::FileHandle(){
    readPageCounter = 0;
    writePageCounter = 0;
    appendPageCounter = 0;
    numPage = 0;
    uid = idGen++;
}

FileHandle::~FileHandle() = default;

void FileHandle::printInf(){
    printf("\n");
    printf("uid : %d \n",uid);
    printf("readPageCounter : \t%d \n",readPageCounter);
    printf("writePageCounter : \t%d \n",writePageCounter);
    printf("appendPageCounter : \t%d \n",appendPageCounter);
    printf("numPage : \t%d,  actSize%%PAGESIZE :\t%d \n",numPage, getFileSize()%PAGE_SIZE);
    printf("filesize : \t%d \n",getFileSize());
    printf("\n");
}

RC FileHandle::setFp(FILE* fp){
    this->fp = fp;

    //need update metadata each time with a not-null new fp
    if(this->fp)
        return updateMetaDataFromFile();

    return 0;
}

FILE* FileHandle::getFp(){
    return fp;
}

RC FileHandle::readPage(PageNum pageNum, void *data) {
    if (pageNum>numPage){
        LOGERR("pageNum too large in readPage")
        return -1;
    }

    if (!fp){
        LOGERR("paged file not yet open in readPage")
        return -1;
    }

    unsigned int offset = getOffsetByPageNum(pageNum);
    //move file ptr
    //todo: check offset valid
    fseek(fp,offset, SEEK_SET);

    if(1 != fread(data, PAGE_SIZE,1,fp)) {
        LOGERR("fread failed in readPage")
        return -1;
    }

    readPageCounter++;

    updateMetaDataToFile();

    return 0;
}

RC FileHandle::writePage(PageNum pageNum, const void *data) {
    if (pageNum>numPage){
        LOGERR("pageNum too large in writePage")
        return -1;
    }

    if (!fp){
        LOGERR("paged file not yet open in writePage")
        return -1;
    }

    unsigned int offset = getOffsetByPageNum(pageNum);
    //move file ptr
    fseek(fp,offset, SEEK_SET);

    if(1 !=fwrite(data, PAGE_SIZE,1,fp)) {
        LOGERR("fwrite failed in writePage")
        return -1;
    }

    writePageCounter++;

    updateMetaDataToFile();

    return 0;
}

RC FileHandle::appendPage(const void *data) {

    if (!fp){
        LOGERR("paged file not yet open in appendPage")
        return -1;
    }

    //unsigned int offset = getOffsetByPageNum(numPage);
    //move file ptr to end
    fseek(fp, 0, SEEK_END);

    if(1 != fwrite(data, PAGE_SIZE,1,fp)) {
        LOGERR("fwrite failed in appendPage")
        return -1;
    }

    appendPageCounter++;
    numPage++;

    updateMetaDataToFile();

    return 0;
}

unsigned FileHandle::getNumberOfPages() {
    return numPage;
}

RC FileHandle::collectCounterValues(unsigned &readPageCount, unsigned &writePageCount, unsigned &appendPageCount) {
    readPageCount = readPageCounter;
    writePageCount = writePageCounter;
    appendPageCount = appendPageCounter;
    return 0;
}

unsigned FileHandle::getOffsetByPageNum(PageNum pageNum){
        //the 0th page is not used, so have to +1
        return (pageNum+1) * PAGE_SIZE;
}

unsigned FileHandle::getFileSize(){
    if(!fp){
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    auto length = ftell(fp);
    rewind(fp);

    return length;
}

RC FileHandle::updateMetaDataToFile(){
    //use the first page
    if(!fp){
        LOGERR("file not yet open in updateMetaDataToFile")
        return -1;
    }

    //back to start
    //fseek(fp,0,SEEK_SET);
    rewind(fp);

    //if it is a new file
    if (PAGE_SIZE>getFileSize()){
        char data[PAGE_SIZE];
        if(1 !=fwrite((const void*)data, PAGE_SIZE, 1, fp)){
            LOGERR("fwrite error when initializing updateMetaDataToFile")
            return -1;
        }
        rewind(fp);
    }

    unsigned int meta[4];
    meta[0] = readPageCounter;
    meta[1] = writePageCounter;
    meta[2] = appendPageCounter;
    meta[3] = numPage;

    if(1 !=fwrite((const void*)meta, sizeof(meta), 1, fp)){
        LOGERR("fwrite error when updateMetaDataToFile")
        return -1;
    }

    rewind(fp);

    return 0;
}

RC FileHandle::updateMetaDataFromFile(){
    if(!fp){
        LOGERR("file not yet open in updateMetaDataFromFile")
        return -1;
    }

    unsigned int meta[4];
    if(getFileSize()<PAGE_SIZE){
        //no need to read, but update to file
        readPageCounter = 0;
        writePageCounter = 0;
        appendPageCounter = 0;
        numPage = 0;
        return updateMetaDataToFile();
    }

    //back to start
    rewind(fp);

    if(1 !=fread(meta, sizeof(meta), 1, fp)){
        LOGERR("fread error when updateMetaDataFromFile")
        return -1;
    }

    readPageCounter = meta[0];
    writePageCounter = meta[1];
    appendPageCounter = meta[2];
    numPage = meta[3];

    rewind(fp);

    return 0;
}

int main(){
    //prepare data
    char data[20][PAGE_SIZE];
    for (int i=0; i<20; i++){
        memset(data[i],i,PAGE_SIZE);
    }

    //use mgr to create and open file, set up fh
    PagedFileManager& mgr = PagedFileManager::instance();
    FileHandle fh;
    mgr.createFile("test.dat");
    mgr.openFile("test.dat",fh);
    
    //use fh to read and write file
    fh.printInf();
    for (int i=0; i<20; i++){
        fh.appendPage((const void*) data[i]);
        fh.printInf();
    }

    for (int i=0; i<20; i++){
        fh.writePage(i,(const void*) data[i]);
        fh.printInf();
    }

    for (int i=0; i<20; i++){
        fh.readPage(i,data[i]);
        fh.printInf();
    }

    return 0;
}