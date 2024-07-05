#include "./dmem.c"


int main(int argc, char const *argv[])
{


    init();


   char writebuffer[PAGE_SIZE];

    /*for(int i = 1; i < 50000; i ++ ) { 
        sprintf(writebuffer, "to page: %d", i);

        printf("writing pgae %s\n", writebuffer);
        writeDBPage(i, writebuffer);
    }*/

    read_mem_page(22, writebuffer);
    read_mem_page(22, writebuffer);
    read_mem_page(22, writebuffer);
    read_mem_page(22, writebuffer);
    read_mem_page(22, writebuffer);
    printf("\n\nvalue in the page: %s\n\n", writebuffer);
}
