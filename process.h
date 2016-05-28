#ifndef process_h
#define process_h

typedef struct {
    int id;
    int num_pages;
    int total_ref;
    int cur_ref;
    int type;
    int locality;
    int alive;
    int prev_work_set;
//    int workSet[MAXSIZE_WORK_SET];
//    int sizeWorkSet;

} process;

/* Print out process START and create process */
void start_process(process *p, int id, int size, int tot_ref, int type, int loc);

/* Returns 0 if no more memory references to make */
int mem_reference(process *p);

/* Print out process TERMINATE and delete process */
void end_process(process *p);

#endif
