#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "types.h"

//void mlfq(Process p[], int n, int cpu_count);
void sjf(Process p[], int n, int cpu_count);
void edf(Process p[], int n, int cpu_count);
void display(Process p[], int n);

#endif
