FLAGS = -Wall

all: lab4 lab5

lab5: lab5.o frametable.o pagetable.o frame.o
	gcc $(FLAGS) -g lab5.o frametable.o pagetable.o frame.o -o lab5 -g

lab4: lab4.o processmix.o process.o
	gcc $(FLAGS) -g lab4.o processmix.o process.o -o lab4

lab5.o: lab5.c frametable.h pagetable.h frame.h
	gcc $(FLAGS) -g -c lab5.c -o lab5.o

lab4.o: lab4.c processmix.h process.h 
	gcc $(FLAGS) -g -c lab4.c -o lab4.o

frametable.o: frametable.c frametable.h frame.h
	gcc $(FLAGS) -g -c frametable.c -o frametable.o

pagetable.o: pagetable.c pagetable.h frametable.h
	gcc $(FLAGS) -g -c pagetable.c -o pagetable.o

frame.o: frame.c frame.h
	gcc $(FLAGS) -g -c frame.c -o frame.o

processmix.o: processmix.c processmix.h process.h
	gcc $(FLAGS) -g -c processmix.c -o processmix.o

process.o: process.c process.h
	gcc $(FLAGS) -g -c process.c -o process.o

clean:
	rm -f *.o lab4 lab5

