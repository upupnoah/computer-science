CC = gcc

CFLAGS = -O1 -Wall

all: sortbench sortbench-lc

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

realtimer.o: realtimer.c realtimer.h
	$(CC) $(CFLAGS) -c realtimer.c

taskq.o: taskq.c taskq.c
	$(CC) $(CFLAGS) -c taskq.c

pqsort.o: pqsort.c pqsort.h taskq.h
	$(CC) $(CFLAGS) -c pqsort.c

pqsort-lc.o: pqsort.c pqsort.h taskq.h
	$(CC) $(CFLAGS) -c pqsort.c -DLOGCOMPS -o pqsort-lc.o

sortbench: sortbench.c csapp.o realtimer.o pqsort.o taskq.o
	$(CC) $(CFLAGS) -o sortbench sortbench.c csapp.o realtimer.o pqsort.o taskq.o -lpthread

sortbench-lc: sortbench.c csapp.o realtimer.o pqsort-lc.o taskq.o
	$(CC) $(CFLAGS) -o sortbench-lc -DLOGCOMPS sortbench.c csapp.o realtimer.o pqsort-lc.o taskq.o -lpthread

sortbench-data.txt: sortbench sortbench-run.pl
	./sortbench-run.pl > sortbench-data.txt

clean:
	rm -rf *~ sortbench sortbench-lc *.o
