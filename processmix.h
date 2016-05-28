#ifndef processmix_h
#define processmix_h
#include "process.h"

typedef struct {
    int num_processes;
    int num_pages;
    int gen_locality;
    int gen_type;
    int gen_variance;
    int gen_pressure;    
    process *processArray;
} processMix;

void initialize_processMix(processMix *pm, int num_procs, int num_pages, int gen_loc, int gen_type, int gen_var, int gen_press);

void generateMemRefs(processMix *pm);

void delete_processMix(processMix *pm);

#endif
