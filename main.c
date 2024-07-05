#include "./pageman.c"


int main(int argc, char const *argv[])
{


init_master_block();


    char writebuffer[PAGE_SIZE];

    for(int i = 1; i < 100; i ++ ) { 
        sprintf(writebuffer, "writing to page: %d", i);
        writeDBPage(i, writebuffer);
    }


    char buf[PAGE_SIZE];
    readDBPage(20, buf);
    printf("\n\nvalue in the page: %s\n\n", buf);
}
