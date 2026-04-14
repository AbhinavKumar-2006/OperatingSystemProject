CC = gcc
CFLAGS = -Wall -g

all: mlfq_sim

mlfq_sim: mlfq.c types.h
	$(CC) $(CFLAGS) -o mlfq_sim mlfq.c

clean:
	rm -f mlfq_sim mlfq_sim.exe
