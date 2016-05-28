#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "processmix.h"
#include "process.h"


void printUsageString();

int main(int argc, char **argv){

    /* Assuming that we can use this program as the second command line argument ... use an unnamed pipe from main program to set this variable */
    //int size_RAM;

    /* process mix */
    processMix pm;

	/* default values for command line arguments */
	int num_processes = 50; //50 total processes in memory
	int locality = 1;       //all processes in mix have avg locality
	int size = 1000;        //this represents total number of virtual pages of all processess combined
    int type = 0;           //most processes in mix are CPU bound
    int variance = 5;       //there is an avg amount of variance between the sizes of the processes
    int mem_pressure = 1;   //avg amount of mem references per process

	/* variables used by getopt()...they are defined in <unistd.h> */
    extern int optopt;
	extern int optind;
	extern char *optarg;
	extern int opterr;
	opterr = 0;

	/* other variables used in parsing command line arguments */
	int c;
	char str[] = "unlstvm"; //used for last case in switch statement

	while((c = getopt(argc, argv, "un:l:s:t:v:m:")) != -1){
		switch(c){
			case 'u':
                printUsageString();
				exit(0);
			case 'n':
                num_processes = strtol(optarg, 0, 0);
                if(num_processes <= 0){
                    fprintf(stderr,"ERROR: (-n) Please input more than 0 processes.\n");
                    exit(0);
                }
				break;
			case 'l':
                locality = strtol(optarg, 0, 0);
                if(locality < 0 || locality > 2){
                    fprintf(stderr,"ERROR: (-l) Please input either 0, 1, or 2.\n");
                    exit(0);
                }
				break;
			case 's':
                size = strtol(optarg, 0, 0); //* size_all_pages;
                if(size <= 0){
                    fprintf(stderr,"ERROR (-s) Please input a size of more than 0.\n");
                }
				break;
			case 't':
                type = strtol(optarg, 0, 0);
                if(type < 0 || type > 1){
                    fprintf(stderr,"ERROR: (-t) Please input either 0 or 1.\n");
                    exit(0);
                }
				break;
			case 'v':
                variance = strtol(optarg, 0, 0);
                if(variance < 0 || variance > 10){
                    fprintf(stderr,"ERROR: (-v) Please input a number between 0 and 10, inclusive.\n");
                    exit(0);
                }
				break;
			case 'm':
                mem_pressure = strtol(optarg, 0, 0);
                if(mem_pressure < 0 || mem_pressure > 2){
                    fprintf(stderr,"ERROR: (-m) Please input either 0, 1, or 2.\n");
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

    /* Initialize process mix based on command line arguments */
    initialize_processMix(&pm, num_processes, size, locality, type, variance, mem_pressure);

    /* Print out memory references */
    generateMemRefs(&pm);

    /* free memory used to store process mix */
    delete_processMix(&pm);

    return 0;
}

void printUsageString(){
    fprintf(stderr,"Usage: input [<options>] [<arguments> ...]\n");
    fprintf(stderr,"Options:\n");
    fprintf(stderr,"-u                  print usage string and exit.\n");
    fprintf(stderr,"-n <num-processes>  total processes in memory.\n");
    fprintf(stderr,"-l <locality>       locality of most processes. 0 = low, 1 = avg, 2 = high.\n");
    fprintf(stderr,"-s <size>           number of virtual pages of all processes combined.\n");
    fprintf(stderr,"-t <type>           type of most processes. 0 = CPU bound, 1 = I/O bound.\n");
    fprintf(stderr,"-v <variance>       variance between processes. 0 = low, 5 = avg, 10 = high.\n");
    fprintf(stderr,"-m <mem-pressure>   memory references per process. 0 = low, 1 = avg, 2 = high.\n");
}
