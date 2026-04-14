#ifndef ADITYA_ALGORITHMS_H
#define ADITYA_ALGORITHMS_H

#include "types.h"

void execute_mlfq_sched(Task task_list[], int total_tasks, int total_cores);
void execute_cfs_sched(Task task_list[], int total_tasks, int total_cores);

#endif
