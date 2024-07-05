#include "./disk.c"

#define MAX_PAGES_IN_DISK 1024
#define MASTER_MAGIC_NUMBER  0xaabb

struct page_master { 
    int magic;
    int totalPages;
    int totalBlocks;
};


struct  disk_page {
    int offset;
    int is_deleted;
    int pageId;
    int padding;
};

typedef struct page_look{
    int block_no; 
    int offset;
    char lookup;
} page_look;


const char* PAGE_FILE = "./_db.ini";
const char* DATA_FILE  = "./_data.ini";
extern struct page_master* get_super_block();
extern void update_super_block(struct page_master* sb);
extern void writeDBPage(int pageId, char* buffer);
extern void readDBPage(int pageId, char* buffer);
extern struct disk_page pull_page(page_look pl);
extern page_look find_by_pageid(int page_id);
extern page_look lookup_free_page(int page_id);



#define NODES_PER_BLOCK BUFFER_SIZE/sizeof(struct disk_page)

void init_master_block() { 
    printf("intializing master block... \n");
    //first block of datafiles contains the superblock

    char page[BUFFER_SIZE];
    readPage(0, PAGE_FILE, page);
    struct page_master* master =  (struct page_master*)page;
    if(master->magic != MASTER_MAGIC_NUMBER){ 
        printf("magic number not found initializing new block... \n");
        struct page_master new_master;
        new_master.magic = MASTER_MAGIC_NUMBER;
        new_master.totalBlocks=1;
        new_master.totalPages=0;
        writePage(0, PAGE_FILE, (char*)&new_master);
    }else { 

    printf("magic number found in super block... \n");
    }
}

struct page_master* get_super_block() { 
    char page[BUFFER_SIZE];
    readPage(0, PAGE_FILE, page);
    struct page_master* master =  (struct page_master*)page;
    return master;
}

void update_super_block(struct page_master* sb) { 
    writePage(0, PAGE_FILE, (char*)sb);
}

void writeDBPage(int pageId, char* buffer) {
    page_look pl_id = find_by_pageid(pageId);
    if(!pl_id.lookup){
        pl_id = lookup_free_page(pageId);
    }
    struct disk_page current_page = pull_page(pl_id);
    writePage(current_page.offset, DATA_FILE, buffer);
}


void readDBPage(int pageId, char* buffer) {
    page_look pl_id = find_by_pageid(pageId);
    struct disk_page current_page = pull_page(pl_id);
    readPage(current_page.offset, DATA_FILE, buffer);
}



struct disk_page pull_page(page_look pl) { 
    char page[BUFFER_SIZE];
    readPage(pl.block_no, PAGE_FILE, page);
    struct disk_page* pages_on_block =  ((struct disk_page*) page);
    return pages_on_block[pl.offset];
}


page_look find_by_pageid(int page_id) { 
    struct page_master* sb = get_super_block();
    page_look pl;
    pl.lookup = 0;
    for(int i=1; i < sb->totalBlocks && !pl.lookup; i++) {  // looking for deleted blocks
        char page[BUFFER_SIZE];
        readPage(i, PAGE_FILE, page);
        struct disk_page* pages_on_block =  ((struct disk_page*) page);
        for(int j = 0; j < NODES_PER_BLOCK && (j*(i)) < sb->totalPages; j++) { 
            struct disk_page current_page = pages_on_block[j];
            if(current_page.pageId == page_id) { 
                pl.lookup = 1;
                pl.block_no = i;
                pl.offset = j;
                break;
            }
        }
    }
    return pl;
}



page_look lookup_free_page(int page_id) {
    struct page_master* sb = get_super_block();
    struct disk_page page_to_save;
    page_look pl;
    pl.lookup = 0;
    for(int i=1; i < sb->totalBlocks && !pl.lookup; i++) {  // looking for deleted blocks

        char page[BUFFER_SIZE];
        readPage(pl.block_no, PAGE_FILE, page);
        struct disk_page* pages_on_block =  ((struct disk_page*) page);
        for(int j = 0; j < NODES_PER_BLOCK  && (j*(i+1)) < sb->totalPages; j++) { 
            struct disk_page current_page = pages_on_block[j];
            if(current_page.is_deleted) { 
                printf("WARNING WARNING WARNING :::::::::::::::::: found a deleted page: %d --- %d\n", current_page.pageId, current_page.is_deleted);
                page_to_save = current_page;
                page_to_save.is_deleted &= 0;
                pl.lookup = 1;
                pl.block_no = i;
                pl.offset = j;
                break;
            }
        }
    }


    if(!pl.lookup){ 

        int tblock =  sb->totalBlocks-1;
        int tpages = sb->totalPages;
        int free_page_space =  0;
        if(sb->totalBlocks > 0) { 
            free_page_space = (tblock*NODES_PER_BLOCK) - tpages;
        }
        if(free_page_space > 0) { 
            
            pl.lookup = 1;
            pl.block_no = sb->totalBlocks-1;
            pl.offset = NODES_PER_BLOCK-free_page_space;
        }else { 
            pl.offset = 0;
            pl.block_no = sb->totalBlocks;
            sb->totalBlocks = sb->totalBlocks+1;
            pl.lookup = 1;
        }
        page_to_save.is_deleted &= 0;
        page_to_save.offset = sb->totalPages;
    }

     


    char page[BUFFER_SIZE];
    readPage(pl.block_no, PAGE_FILE, page);
    struct disk_page* pages_on_block =  ((struct disk_page*) page);
    page_to_save.pageId = page_id;
    pages_on_block[pl.offset] = page_to_save;

    
    writePage(pl.block_no, PAGE_FILE, (char*)pages_on_block);
    sb->totalPages = sb->totalPages+1;
    
    update_super_block(sb);
    return pl;
}

