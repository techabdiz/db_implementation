#include "./pageman.c"


int main(int argc, char const *argv[])
{


init_master_block();
   // for(int i = 0; i < 120; i++) {
    writeDBPage(50, "please save this to the db");
   // char buf[PAGE_SIZE];
   //readDBPage(1, buf);
   //printf("value in the page: %s\n", buf);

   // }

    

    return 0;
}
