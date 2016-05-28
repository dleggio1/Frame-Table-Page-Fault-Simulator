Project Members:

Jared Rosenberg
Dylan Leggio
Daniel Martin

DIRECTIONS ON HOW TO COMPILE lab4 and lab5:
>lab4:
    make lab4

>lab5:
    make lab5

DIRECTIONS ON HOW TO RUN lab4 and lab5:
>lab4:
    ./lab4 [-u] [-n <num_processes>] [-l <locality>] [-s <size>] [-t <type>] [-v <variance>] [-m <num_references>] > "output.txt" //redirect as needed as of now

    The meaning of each optional argument is as follows:
    -u  print usage string and exit.
    -n <num_processes>  number of processes in the system.
    -l <locality>   locality of the processes in the mix. The viable choices for the argument are 0, 1, or 2. 0 corresponds to a process mix with 90% of processes having low locality and 10% of processes having average locality. 1 corresponds to all processes having average locality. 2 corresponds to 90% of processes having high locality and 10% of processes having average locality. The exact definition of locality was chosen by us, i.e. low locality means that working set consists of all pages in process whereas high locality means that working set consists of a very small subset of pages in the process.  
    -s <size>   number of virtual pages of all processes combined.
    -t <type>   whether the processes in the mix are interactive(IO Bound) or not(CPU Bound). The viable choices for the argument are 0 or 1. 0 corresponds to 90% of processes being CPU bound, i.e. having a lot of memory references during its time slice. 1 corresponds to 90% of processes being IO bound, i.e. having only a few memory references before relinquishing the CPU to another process.  
    -v <variance>   variance in the sizes of the processes in the mix. The viable choices for the argument are an integer between 0 and 10 inclusive. 0 corrsponds to all processes having the same size. 10 corresponds to all processes having different sizes (in range from 1 page to double the average process size). As the argument gets larger in the range from 0 - 10, the process sizes are more variable.
    -m <num_references> number of memory references that the processes make. The viable choices for the argument are 0, 1, or 2. 0 corresponds to 90% of processes in mix having a low number of total memory references (chosen arbitrarily to be 100). 1 corresponds to all processes having an average number of total memory references (chosen arbitrarily to be 200). 2 corresponds to 90% of processes having a high number of memory references (chosen arbitrarily to be 300).

>lab5:
    ./lab5 <frames-of-memory> "output.txt" [-p <num_prefetch>] [-e <eviction_policy>]

	-p <num_prefetch>] prefetching.  Adds page replacement with prefectching the specified number of pages from 0 to 5.
	-e <eviction_policy> options are 0 for FIFO queue page replacement or 1 for clock LRU approximation.

