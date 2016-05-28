#include "processmix.h"
#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>

#define MAX_VAR     10
#define CPU_BOUND   0
#define IO_BOUND    1
#define LOW_LOC     0
#define AVG_LOC     1
#define HIGH_LOC    2
#define LOW_PRESS   0
#define AVG_PRESS   1
#define HIGH_PRESS  2
#define BASE_TOT_REF    100

void initialize_processMix(processMix *pm, int num_procs, int num_pages, int gen_loc, int gen_type, int gen_var, int gen_press){
    pm->num_processes = num_procs;
    pm->num_pages = num_pages;
    pm->gen_locality = gen_loc;
    pm->gen_type = gen_type;
    pm->gen_variance = gen_var;
    pm->gen_pressure = gen_press;
    pm->processArray = (process *)malloc(sizeof(process) * num_procs);
    if(pm->processArray == NULL){
        perror(NULL);
    }

    /* local variables used for initialization of process mix */
    srand(time(0));
    int i;
    int cur_proc_size, cur_proc_type, cur_proc_loc, cur_proc_pressure, avg_proc_size, pages_remaining, temp_procs_left;
    avg_proc_size = num_pages/num_procs;
    pages_remaining = num_pages;

    /* This for loop initializes each process in the process mix */
    for(i = 0; i < num_procs; i++){

        temp_procs_left = num_procs - i - 1; /* temp variable to make calculations easier to read */

        /* use the variance specified by user to calculate the size of each process...sizes vary from one page to double the avg process size */
        cur_proc_size = ((rand() % MAX_VAR) < gen_var) ? ((double)(rand() % 200) / (double)100) * (double)avg_proc_size : avg_proc_size;
        if(!cur_proc_size)
            cur_proc_size++;
        if (cur_proc_size > (pages_remaining - temp_procs_left))
            cur_proc_size = pages_remaining - temp_procs_left;
        pages_remaining -= cur_proc_size;

        /* use the type variable specified by the user to set the process type ... if user specifies CPU_BOUND, have 90% of processes in mix be CPU bound; if user specifies IO_BOUND, have 90% of processes in mix be IO bound. The exact definition of what these mean are in the README file. */
        if(gen_type == CPU_BOUND)
            cur_proc_type = ((rand() % 10) < 9) ? CPU_BOUND : IO_BOUND;
        else if(gen_type == IO_BOUND)
            cur_proc_type = ((rand() % 10) < 9) ? IO_BOUND : CPU_BOUND;


        /* locality ... if user specifies HIGH_LOC, have 90% of processes in mix have high locality (the other 10% having avg locality); if user specifies AVG_LOC, have all processes have avg locality; if user specifies LOW_LOC, have 90% of processes in mix have low locality (the other 10% having avg locality). The exact definition of what each of these mean will be in the README file. */
        if(gen_loc == HIGH_LOC)
            cur_proc_loc = ((rand() % 10) < 9) ? HIGH_LOC : AVG_LOC;
        else if(gen_loc == AVG_LOC)
            cur_proc_loc = AVG_LOC;
        else
            cur_proc_loc = ((rand() % 10) < 9) ? LOW_LOC : AVG_LOC;

        /* memory pressure or number of page references ... if user specifies HIGH_PRESS, 90% of processes have high number of memory references; if user specifies AVG_PRESS, all processes have avg number of memory references; if user specifies LOW_PRESS, 90% of processes have low number of memory references. The exact definition of what each of these mean will be in the README file. */
        if(gen_press == HIGH_PRESS)
            cur_proc_pressure = ((rand() % 10) < 9) ? (HIGH_PRESS+1)*BASE_TOT_REF : (AVG_PRESS+1)*BASE_TOT_REF;
        else if(gen_press == (AVG_PRESS+1)*BASE_TOT_REF)
            cur_proc_pressure = (AVG_PRESS+1)*BASE_TOT_REF;
        else
            cur_proc_pressure = ((rand() % 10) < 9) ? (LOW_PRESS+1)*BASE_TOT_REF : (AVG_PRESS+1)*BASE_TOT_REF;

        start_process((pm->processArray) + i, i, cur_proc_size, cur_proc_pressure, cur_proc_type, cur_proc_loc);
    }
}

void generateMemRefs(processMix *pm){

    int i, cur_index, finish_flag;
    srand(time(0));
    finish_flag = 0;
    while(!finish_flag){
    /* Randomly choose a process to start making memory references */
        cur_index = rand() % (pm->num_processes);
        if(!((pm->processArray + cur_index)->alive)){
            i = cur_index+1;
            finish_flag = 1;
            while((i % (pm->num_processes)) != cur_index){
                if((pm->processArray + (i%(pm->num_processes)))->alive){
                    cur_index = i % (pm->num_processes);
                    finish_flag = 0;
                    break;
                } else i++;
            }
        }
        if (!finish_flag){
            mem_reference((pm->processArray)+cur_index);
        }
    }
}

void delete_processMix(processMix *pm){
    free((pm->processArray));
}


