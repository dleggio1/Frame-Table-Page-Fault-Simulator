#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#define BASE_CPU_REF    80
#define BASE_IO_REF     20
#define LOW_LOC         0
#define AVG_LOC         1
#define HIGH_LOC        2
#define AVG_LOC_FACTOR  8
#define HIGH_LOC_FACTOR 16
#define DEAD    0
#define ALIVE   1

/* ex: a process with 20 pages has pages numbered 0 - 19 */

void start_process(process *p, int id, int size, int tot_ref, int type, int loc){
    p->id = id;
    p->num_pages = size;
    p->total_ref = tot_ref;
    p->cur_ref = 0;
    p->type = type;
    p->locality = loc;
    p->alive = ALIVE;
    return;
}

int mem_reference(process *p){
    int counter;
    int limit;
    int range;
    int base_work_set; 
    if(p->cur_ref == 0){
        fprintf(stdout, "START %d %d\n", p->id, p->num_pages);
        (p->cur_ref)++;
    }
    counter = 0;
    base_work_set = 0;

    /* if process is IO bound, reference fewer pages during time slice compared to CPU bound */
    limit = (p->type) ? BASE_IO_REF : BASE_CPU_REF; 

    /* set size of current working set depending on process's locality ... the higher the locality, the smaller the number of pages in the working set */
    if(p->locality == LOW_LOC)
        range = (p->num_pages);
    else if(p->locality == AVG_LOC)
        range = (p->num_pages / AVG_LOC_FACTOR)+1;
    else
        range = (p->num_pages / HIGH_LOC_FACTOR)+1;
    
    /* choose a new working set for current time slice */
    if(range < (p->num_pages))
        while((base_work_set = (rand() % p->num_pages)) > (p->num_pages - (range+1)));
    /* if process has high locality, give it a higher probability of remaining in same working set as previous time slice */
    if (p->cur_ref > 1){
        if((p->locality == HIGH_LOC) && ((rand() % 10) > 7))
            base_work_set = p->prev_work_set;
    }
    /* generate memory references within working set */
    while(counter < limit){
        if(p->cur_ref > p->total_ref){
            end_process(p);
            return 0;
        }
        printf("REFERENCE %d %d\n", p->id,(rand() % range) + base_work_set);
        (p->cur_ref)++;
        counter++;  
    }
    p->prev_work_set = base_work_set;
    return 1;
}


void end_process(process *p){
    p->alive = DEAD;
    fprintf(stdout, "TERMINATE %d\n", p->id);
    return;
}
