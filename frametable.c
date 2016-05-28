#include "frametable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

struct node *create_node(int n){
    node *temp = (node*)malloc(sizeof(node));
    temp->data = n;
    temp->next = NULL;
    return temp;
}

struct queue *init_queue(){
    queue *q = (queue*)malloc(sizeof(queue));
    q->front = q->rear = NULL;
    return q;
}


void enqueue(queue *q, int n){
    node *temp = create_node(n);

    if(q->rear == NULL){
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

int dequeue(queue *q){

    int pagenum;

    if(q->front == NULL){
        fprintf(stderr,"ERROR: queue is empty.\n");
        exit(0);
    }

    node *temp = q->front;
    q->front = q->front->next;

    if(q->front == NULL){
        q->rear = NULL;
    }

    pagenum = temp->data;
    free(temp);
    return pagenum;

}

int lru(frameTable *ft){
	int evicted = -1;
	while(evicted == -1){
		if(!getFrameUsed(ft->frames + ft->hand)){
			evicted = ft->hand;
		}else{
			setFrameUsed(ft->frames + ft->hand, false);
		}
		if(ft->hand != ft->num_frames - 1) ft->hand++;
		else ft->hand = 0;
	}
	return evicted;
}

void initialize_ft(frameTable *ft, int num_frames){
    int i;
    ft->num_frames = num_frames;
    ft->frames = (frame *)malloc(sizeof(frame) * num_frames);
    for(i = 0; i < num_frames; i++){
        initializeFrame(ft->frames + i);
    }
    q = init_queue();
		ft->hand = 0;
}

int findFree(frameTable *ft){
    int i;
    for(i = 0; i < ft->num_frames; i++){
        if(getFramePid(ft->frames + i) == -1){
            return i;
        }
    }
    return -1;
}

void freePages(frameTable *ft, int p_id){
    int i;
    for(i = 0; i < ft->num_frames; i++){
        if(getFramePid(ft->frames + i) == p_id){
            initializeFrame(ft->frames + i);
        }
    }
}

void delete_ft(frameTable *ft){
    free(ft->frames);
}
