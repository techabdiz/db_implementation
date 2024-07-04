#include <stdio.h>
#define BUFFER_SIZE 4096
#define PAGE_SIZE 20


struct page { 
    char fixed; // page in use 
    char buff[BUFFER_SIZE];
    char dirty; // page is dirty
    char flushed; // page is flushed
    int pageNo; // page no on disk
    int counter; // counter for LRU
};


int pageAllocationCounter = 0;

struct page pages[PAGE_SIZE];

struct page getFreeSlot(){ 
    int freeSlot = getFreeSlotIndex();
    if(freeSlot == -1) { 
        freeSlot = lru_page();
    }
    return clean_dirty_page(freeSlot);
}


struct page clean_dirty_page(int dirtyPage) { 
    struct page freePage = pages[dirtyPage];
    if(freePage.dirty && !freePage.flushed) { 
        // writePageToDisk();
    }
    freePage.dirty = 0;
    freePage.flushed = 1;
    freePage.fixed = 1;
    freePage.counter = pageAllocationCounter++;
    return pages[dirtyPage];
}


char page_in_buffer(int pageNo){
    for(int i = 0; i < PAGE_SIZE; i++) { 
        if(pages[i].pageNo == pageNo){ 
            return 1;
        }
        return 0;
    }
}

int lru_page() { 
    int smallestCount = 0;
    for(int i = 0; i < PAGE_SIZE; i++) { 
        if(i == 0 ) { 
            smallestCount = pages[i].counter;
        }else { 
            if(smallestCount < pages[i].counter) { 
                smallestCount = pages[i].counter;
            }
        }
    }
    return smallestCount;
}

int getFreeSlotIndex(){ 
    int index = -1;
    for(int i = 0; i < PAGE_SIZE; i++) { 
        if(!pages[i].fixed){
            index = i;
            break;
        }
    }
    return index;
}


