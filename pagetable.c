#include "pagetable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
/* an index in page table with a value in -1 means that the page is not currently in memory */

void new_pt(pageTable *pt, int pid, int num_pages){
    int i;
    pt->pid = pid;
    pt->size = num_pages;
    pt->pages = (int *)malloc(sizeof(int)*num_pages);
    for(i = 0; i < num_pages; i++){
        *(pt->pages+i) = -1;
    }
}
int inMem(pageTable *pt, int page){
    return (*(pt->pages + page));
}

void assignPage(pageTable *pt, frameTable *ft, int page_num, int frame_num){
    *(pt->pages+page_num) = frame_num;
    setFramePid(ft->frames + frame_num, pt->pid);
    setFramePage(ft->frames + frame_num, page_num);
		setFrameUsed(ft->frames + frame_num, true);
    enqueue(q,frame_num);
}


void delete_pt(pageTable *pt){
    free(pt->pages);
}
