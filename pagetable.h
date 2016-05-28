#ifndef pageTable_h
#define pageTable_h
#include "frametable.h"

/* A page table is just an array of integers. The index into the array is the virtual page number and the value at that index is the physical frame that it maps to. If the value is -1, that means the page is not currently in physical memory */
typedef struct{
    int pid;
    int size;
    int *pages;
} pageTable;

void new_pt(pageTable *pt, int pid, int num_pages);

/* Returns -1 if not in mem ... returns frame number if in mem */
int inMem(pageTable *pt, int page);

void assignPage(pageTable *pt, frameTable *ft, int page_num, int frame_num);

void delete_pt(pageTable *pt);


#endif

