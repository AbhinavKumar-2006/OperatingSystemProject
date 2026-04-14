#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
  int pid;
  int i;

  for(i = 1; i <= 6; i++) {
    pid = myfork();
    if(pid < 0) break;
    if(pid == 0) exit(0);
    printf("parent %d created child %d pid=%d\n", getpid(), i, pid);
  }
  while(wait(0) >= 0);

  exit(0);
}

//test file to check functionality of the custom myfork system call, which limits the number of child processes that can be created by a parent process to 5. The program attempts to create 6 child processes, and the expected output is that the first 5 child processes are created successfully, while the 6th attempt should fail with a message indicating that the maximum number of child processes has been reached.
//written by Aditya Ekka
