#ifndef MY_ALGORITHMS_H
#define MY_ALGORITHMS_H

#include "types.h"

void execute_lottery_sched(Task task_list[], int total_tasks, int total_cores);
void execute_rms_sched(Task task_list[], int total_tasks, int total_cores);

#endif
