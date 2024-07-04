#include "./dmem.c"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


#define MAX_PAGES_IN_DISK 1024

const char* META_FILE = "./_db.ini";
const char* DATA_FILE  = "./_data.ini";

struct  disk_node {
    int offset;
    char buffer[PAGE_SIZE];
    int is_deleted;
    int pageNo;
};

int writePage(buffer, diskPageNo) { 

}



char* readPage(int pageNo, const char* filesName) { // pool the page buffer
    char buffer[PAGE_SIZE];
    int distance = pageNo*PAGE_SIZE;
    int fd =  open (filesName, O_RDONLY);
    lseek(fd, distance, 0);
    read(fd, buffer, PAGE_SIZE);
    close(fd);
}