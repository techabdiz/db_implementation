#include "./dmem.c"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


extern void seekToPage(int pageNo, int fd);

int writePage(int pageNo, const char* filesName, char* buffer) { 
    int fd =  open (filesName, O_RDWR);
    seekToPage(pageNo, fd);
    int nbytes = write(fd, buffer, BUFFER_SIZE);
    close(fd);
}

int readPage(int pageNo, const char* filesName, char* buffer) { // pool the page buffer
    int fd =  open (filesName, O_RDONLY);
    seekToPage(pageNo, fd);
    int bytesRead = read(fd, buffer, BUFFER_SIZE);
    close(fd);
    return bytesRead;
}

void seekToPage(int pageNo, int fd) { 
    int distance = pageNo*BUFFER_SIZE;
    lseek(fd, distance, 0);
}
