#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "vm.h"

extern struct proc proc[NPROC];



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
<<<<<<< HEAD
//eb
uint64 sys_getpriority(void) {
    return myproc()->priority;
}

uint64 sys_setpriority(void) {
    int p;
    argint(0, &p);
    myproc()->priority = p;
    return 0;
}

uint64 sys_send(void)
{
    int pid, msg;
    argint(0, &pid);
    argint(1, &msg);

    struct proc *p;

    for(p = proc; p < &proc[NPROC]; p++)
    {
        acquire(&p->lock);

        if(p->pid == pid)
        {
            p->msg = msg;
            p->has_msg = 1;
            release(&p->lock);
            return 0;
        }

        release(&p->lock);
    }

    return -1;
}

uint64 sys_recv(void) {
    struct proc *p = myproc();

    while(p->has_msg == 0) {
        ; // wait
    }

    int m = p->msg;
    p->has_msg = 0;
    return m;
}

uint64 sys_signal(void) {
    int pid;
    argint(0, &pid);

    return kkill(pid);
}
//endeb
=======

uint64 sys_myfork(void) { //custom system call implementation by Aditya Ekka
  int count = 0;
  struct proc *p;
  struct proc *cur = myproc();
  const int max = 5;

  for(p = proc; p < &proc[NPROC]; p++){
    if(p->parent == cur)
      count++;
  }

  if(count >= max){
    printf("Maximum number of child processes reached. Cannot fork.\n");
    return -1;
  }
  return kfork();
}
>>>>>>> 8b3a5f4
