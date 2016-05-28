#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "pagetable.h"
#include "frametable.h"
#include "frame.h"

#define MAXSIZE 64

void replacement(int prefetching, int eviction, frameTable *ft, pageTable *ptables, int pid, int page_num);
int evictPage(int eviction, frameTable *ft, pageTable *ptables);
void printUsageString(char *name);

int main(int argc, char **argv){

    FILE *input;
    int proc_counter;
    char buffer[MAXSIZE];
    int buf_pid, buf_num_pages, buf_page_num, free_frame;
    int pagefaults, pagehits, tot_refs;
    int prefetching, eviction;

    /* array of page tables...the index of the array represents the process number and the value at that index is its page table */
    pageTable *ptables;

    /* frame table */
    frameTable ftable;

    prefetching = 0;    /* default value for prefetching policy */
    eviction = 0;       /* default value for replacement policy  */

    extern int optopt;
	extern int optind;
	extern char *optarg;
	extern int opterr;
	opterr = 0;

	/* other variables used in parsing command line arguments */
	int c, index;
	char str[] = "upe"; //used for last case in switch statement

	while((c = getopt(argc, argv, "up:e:")) != -1){
		switch(c){
			case 'u':
                printUsageString(argv[0]);
				exit(0);
			case 'p':
                prefetching = strtol(optarg, 0, 0);
                if(prefetching < 0 || prefetching > 5){
                    fprintf(stderr,"ERROR: (-p) Please input a value between 0 and 5 inclusive\n");
                    exit(0);
                }
				break;
			case 'e':
                eviction = strtol(optarg, 0, 0);
                if(eviction < 0 || eviction > 1){
                    fprintf(stderr,"ERROR: (-e) Please input a value between 0 and 1 inclusive\n");
                    exit(0);
                }
				break;
			case '?':
                /* double check these */
				if (strchr(str, optopt)) fprintf(stderr, "Option -%c requires an argument. Program exiting...\n", optopt);
				else if (isprint(optopt)) fprintf(stderr, "Unknown option -%c. Program exiting... \n", optopt);
				else fprintf(stderr, "Unknown option character. Program exiting... \n");
				exit(1);
		}
	}

    index = optind;

    /* initialize frame table */
    initialize_ft(&ftable, strtol(argv[index], NULL, 0));
    index++;
    
    /* open the input file */
    input = fopen(argv[index], "r");
    if(input == NULL){
        perror(NULL);
    }

    /* Do an initial run through to get the number of processes in the reference string */
    proc_counter = 0;
    while(fgets(buffer, MAXSIZE, input) != NULL){
        if(buffer[0] == 'S') proc_counter++;
    }
    rewind(input);

    /* Allocate space for page tables */
    ptables = (pageTable *)malloc(sizeof(pageTable) * proc_counter);
    if(ptables == NULL){
        perror(NULL);
    }

    /* keep track of page fault rate */
    pagefaults = 0;
    pagehits = 0;
    tot_refs = 0;

    /* process reference string and simulate virtual memory system */
    while(fgets(buffer, MAXSIZE, input) != NULL){
        if(buffer[0] == 'S'){ /* start new process  */
            sscanf(buffer+5, "%d %d", &buf_pid, &buf_num_pages);
            /* initialize new page table for the new process */
            new_pt(ptables + buf_pid, buf_pid, buf_num_pages);

        } else if(buffer[0] == 'R'){ /* memory reference  */
            sscanf(buffer+9, "%d %d", &buf_pid, &buf_page_num);
            tot_refs++;
            if(inMem(ptables+buf_pid, buf_page_num) < 0){
                /* PAGE FAULT */
                pagefaults++;
                if((free_frame = findFree(&ftable)) < 0){ /* run page replacement algorithm here */
                    replacement(prefetching, eviction, &ftable, ptables, buf_pid, buf_page_num);
                } else { /* replace free page */
                    assignPage(ptables+buf_pid, &ftable, buf_page_num, free_frame);
                }
            } else {
                /* PAGE HIT */
                pagehits++;
            }

        } else { /* terminate process  */
            if(sscanf(buffer+9, "%d", &buf_pid) != 1)
                perror(NULL);
            /* free current process's entries in frame table */
            freePages(&ftable, buf_pid);
            /* delete page table for the process */
            delete_pt(ptables+buf_pid);
        }
    }
    fprintf(stdout, "Page Faults: %d\nPage Hits: %d\nTotal References: %d\nPage Fault Rate: %f\n", pagefaults, pagehits, tot_refs, (double)pagefaults/(double)tot_refs);
    delete_ft(&ftable);
    free(ptables);

    fclose(input);

    return 0;

}


void replacement(int prefetching, int eviction, frameTable *ft, pageTable *ptables, int pid, int page_num){
    int i, num_evicted, size, free_frame;
    size = (ptables + pid)->size;
    switch(prefetching){
        case 0:
                if((size - page_num) > 1)
                    num_evicted = 1;
                else
                    num_evicted = size-page_num;
                break;
        case 1:
                if((size - page_num) > 2)
                    num_evicted = 2;
                else
                    num_evicted = size-page_num;
                break;
        case 2:
                if((size - page_num) > 3)
                    num_evicted = 3;
                else
                    num_evicted = size-page_num;
                break;
        case 3:
                if((size - page_num) > 4)
                    num_evicted = 4;
                else
                    num_evicted = size-page_num;
                break;
        case 4:
                if((size - page_num) > 5)
                    num_evicted = 5;
                else
                    num_evicted = size-page_num;
                break;
        case 5:
                if((size - page_num) > 6)
                    num_evicted = 6;
                else
                    num_evicted = size-page_num;
                break;
        default:
                break;
    }

    for(i = 0; i < num_evicted; i++){
        if(inMem(ptables + pid, page_num) < 0){
            free_frame = evictPage(eviction, ft, ptables);
            assignPage(ptables + pid, ft, page_num, free_frame);
        }
        page_num++;
    }
}


int evictPage(int eviction, frameTable *ft, pageTable *ptables){
    int evict_frame;    /* frame number that is evicted   */
    int evict_pid;      /* process whose page is evicted */
    int evict_page;     /* page number of process thats evicted */
    if(eviction == 0){ /*  FIFO   */
        /* just need you to use FIFO to select a frame number to evict and set it equal to evict_frame ... I currently just have it choosing a random frame to evict from */
        //evict_frame = rand() % (ft->num_frames - 1);
        evict_frame = dequeue(q);


    } else { /* LRU   */

        /* just need you to use LRU to select a frame number to evict and set it equal to evict_frame ... I currently just have it choosing a random frame to evict from */
        evict_frame = lru(ft);

    }

    /* update page table that you evicted page from */
    evict_pid = getFramePid(ft->frames + evict_frame);
    evict_page = getFramePage(ft->frames + evict_frame);
    *((ptables + evict_pid)->pages + evict_page) = -1;

    return evict_frame;
}

void printUsageString(char *name){
    fprintf(stderr,"Usage: %s [<options>] <num-frames> <input file>\n", name);
    fprintf(stderr,"Options:\n");
    fprintf(stderr,"-u                      print usage string and exit.\n");
    fprintf(stderr,"-p <prefetch policy>    prefetch policy.\n");
    fprintf(stderr,"-e <page eviction>      page eviction policy.\n");
}
