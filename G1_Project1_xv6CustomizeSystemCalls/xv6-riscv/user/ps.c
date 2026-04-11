#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int active = getprocs();
  
  printf("System Diagnostics:\n");
  printf("Total active processes: %d\n", active);
  
  exit(0);
}