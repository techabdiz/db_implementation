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

   print_all_pages();

    for(int i = 1; i < 25; i++ ) { 
        read_mem_page(i, writebuffer);
    }

    read_mem_page(5, writebuffer);
    read_mem_page(303, writebuffer);


    print_all_pages();
    printf("\n\nvalue in the page: %s\n\n", writebuffer);
}
