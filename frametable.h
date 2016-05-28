#ifndef frameTable_h
#define frameTable_h
#include "frame.h"
/* the index into the frameTable->frames array is the physical frame number and the value at that index is the process id number that is currently occupying that frame. If the value at an index is -1, that means the frame is free */

typedef struct node{
    int data;
    struct node *next;
} node;

typedef struct queue{
    node *front;
    node *rear;
} queue;

queue *q;

struct node *create_node(int n);
struct queue *init_queue();
void enqueue(queue *q, int n);
int dequeue(queue *q);

typedef struct{
    int num_frames;
    frame *frames;
		int hand;
} frameTable;

void initialize_ft(frameTable *ft, int num_frames);

/* returns -1 if no frames are free ... returns frame number that is free if one is free */
int findFree(frameTable *ft);

void freePages(frameTable *ft, int p_id);

int lru(frameTable *ft);

void delete_ft(frameTable *ft);



#endif
