#include <stdio.h>
#include <string.h>
#include "./pageman.c"

#define BUFFER_SIZE 4096
#define PAGE_SIZE 4096
#define new_max(x,y) (((x) >= (y)) ? (x) : (y))
#define new_min(x,y) (((x) <= (y)) ? (x) : (y))

struct page { 
    char fixed; // page in use 
    char buff[BUFFER_SIZE];
    char dirty; // page is dirty
    char flushed; // page is flushed
    int pageNo; // page no on disk
    int counter; // counter for LRU
};
extern struct page* getFreeSlot();
extern int getFreeSlotIndex();
extern int lru_page() ;
extern char page_in_buffer(int pageNo);
extern char page_in_buffer(int pageNo);
extern struct page* clean_dirty_page(int dirtyPage);
struct page* get_page(int pageNo);


int pageAllocationCounter = 0;

struct page pages[PAGE_SIZE];

void init() { 
    init_master_block();
}

int read_mem_page(int page_no, char* buff) { 
    if(page_in_buffer(page_no)) {
        get_page(page_no)->counter = pageAllocationCounter++;
        memcpy(buff,get_page(page_no)->buff, PAGE_SIZE);
        return 1;
    }
    struct  page* p = getFreeSlot();
    p->pageNo = page_no;
    readDBPage(page_no, p->buff);
    memcpy(buff,p->buff, PAGE_SIZE);
}

int write_mem_page(int page_no, char* buf) { 
    if(page_in_buffer(page_no)) {
        memcpy(buf, get_page(page_no)->buff, BUFFER_SIZE);
        return 1;
    }
    char temp_buff[BUFFER_SIZE];
    read_mem_page(page_no, temp_buff); // bringing page in memory
    if(page_in_buffer(page_no)) {
        memcpy(get_page(page_no)->buff, buf, BUFFER_SIZE);
        return 1;
    }
    return -1;
   
}

void print_page_details(struct page p) {
    printf("{fixed: %d, dirty: %d, flushed: %d, pageNo: %d, counter: %d}\n", p.fixed, p.dirty, p.flushed, p.pageNo, p.counter);
}
void print_all_pages() { 
    for ( int i = 0; i < PAGE_SIZE; i++) {
        print_page_details(pages[i]);
    } 
}



struct page* getFreeSlot(){ 
    /**
     * TODO: deal with pointers only.. references are copying up the values and eating them
     */
    int freeSlot = getFreeSlotIndex();
    if(freeSlot == -1) { 
        freeSlot = lru_page();
    }
    return clean_dirty_page(freeSlot);
}


struct page* clean_dirty_page(int dirtyPage) { 
    struct page freePage = pages[dirtyPage];
    if(freePage.dirty && !freePage.flushed) { 
        writeDBPage(freePage.pageNo, freePage.buff);
    }
    freePage.dirty = 0;
    freePage.flushed = 1;
    freePage.fixed = 1;
    freePage.counter = pageAllocationCounter++;
    pages[dirtyPage] = freePage;
    return &(pages[dirtyPage]);
}


char page_in_buffer(int pageNo){
   // printf("looking for page%d in memory\n", pageNo);
    for(int i = 0; i < PAGE_SIZE; i++) { 
        if(pages[i].pageNo == pageNo){ 
           // printf("found page %d in memory index %d\n", pageNo, i);
            return 1;
        }
    }
    //printf("---------> no page found for pageNo: %d\n", pageNo);
    return 0;
}


struct page* get_page(int pageNo){
    for(int i = 0; i < new_min(pageAllocationCounter, PAGE_SIZE); i++) { 
        if(pages[i].pageNo == pageNo){ 
            return &pages[i];
        }
    }
}

int lru_page() { 
    int smallestCount = 0;
    int smallestIndex = 0;
    for(int i = 0; i < new_min(pageAllocationCounter, PAGE_SIZE); i++) { 
        if(i == 0 ) { 
            smallestCount = pages[i].counter;
        }else { 
            if(smallestCount > pages[i].counter) { 
                smallestCount = pages[i].counter;
                smallestIndex = i;
            }
        }
    }
    return smallestIndex;
}

int getFreeSlotIndex(){ 
    int index = -1;
    for(int i = 0; i < new_min(pageAllocationCounter, PAGE_SIZE); i++) { 
        if(!pages[i].fixed){
            index = i;
            break;
        }
    }
    return index;
}


