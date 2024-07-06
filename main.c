#include "./dmem.c"
#include <stdlib.h> 
#include <time.h> 
 
int printRandoms(int n) 
{ 
    if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    int end = RAND_MAX / n; 
    end *= n;
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
} 

void execute() {

   char writebuffer[PAGE_SIZE];

   int generate_random(int min, int max){
        return rand() % (max - min + 1) + min;
    }

   //print_all_pages();

   // write_mem_page(751, "writing value to the page");

    for(int i = 1; i < 10000; i++ ) {
        int rando =  generate_random(1, 10000);
        
        read_mem_page(rando, writebuffer);
        if(rando == 751) { 
          printf("\nvalue in the page: %s\n\n", writebuffer);
        }
    }

  //  printf("\nvalue in the page: %s\n\n", writebuffer);
    //read_mem_page(303, writebuffer);

   // printf("\nvalue in the page: %s\n\n", writebuffer);

   // print_all_pages();
} 


int main(int argc, char const *argv[])
{
    srand(time(NULL));

    init();
    execute();
    printf("----------------------------------------------------\ntotal disk ops --> %d\n----------------------------------------------------\n", disk_op_counter);
}
