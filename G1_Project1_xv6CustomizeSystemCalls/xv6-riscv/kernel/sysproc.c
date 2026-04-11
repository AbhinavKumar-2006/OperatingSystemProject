#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"



#define NSEM 10 // Maximum number of active semaphores

struct semaphore {
  struct spinlock lock; // Protects the semaphore from race conditions
  int value;            // The current count
  int active;           // 1 if allocated, 0 if free
} semaphores[NSEM];

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_getprocs(void)
{
  return get_active_procs();
}



// CREATE A SEMAPHORE
uint64
sys_sem_create(void)
{
  int initial_val;

  
  argint(0, &initial_val);

  if(initial_val < 0)
    return -1;

  for(int i = 0; i < NSEM; i++) {
    if(semaphores[i].active == 0) {
      initlock(&semaphores[i].lock, "semaphore");
      semaphores[i].value = initial_val;
      semaphores[i].active = 1;
      return i;
    }
  }
  return -1;
}

// DECREMENT (WAIT)
uint64
sys_sem_down(void)
{
  int id;

  argint(0, &id);

  if(id < 0 || id >= NSEM || semaphores[id].active == 0)
    return -1;

  acquire(&semaphores[id].lock);

  while(semaphores[id].value == 0) {
    sleep(&semaphores[id], &semaphores[id].lock);
  }

  semaphores[id].value--;

  release(&semaphores[id].lock);
  return 0;
}

// INCREMENT (SIGNAL)
uint64
sys_sem_up(void)
{
  int id;

  argint(0, &id);

  if(id < 0 || id >= NSEM || semaphores[id].active == 0)
  return -1;

  acquire(&semaphores[id].lock);
  semaphores[id].value++;
  
  // Wake up any processes that are sleeping on this specific semaphore memory address
  wakeup(&semaphores[id]); 
  
  release(&semaphores[id].lock);
  return 0;
}


uint64 sys_clone(void) {
  uint64 stack;
  argaddr(0, &stack);
  return clone(stack);
}

uint64 sys_join(void) {
  return join();
}