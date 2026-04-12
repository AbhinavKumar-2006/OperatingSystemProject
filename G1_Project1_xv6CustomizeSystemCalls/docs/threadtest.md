The threadtest user program is designed to verify the correct implementation of the clone() and join() system calls. It explicitly tests whether a newly created thread successfully shares the exact same physical memory (page table) as its parent process.

How it works:
The program initializes a global shared variable (shared_counter) and allocates a dedicated 4KB block of memory to serve as the new thread's independent user stack. It then invokes the clone() system call. The spawned thread modifies the global variable and exits, while the parent process uses join() to block and wait for the thread's completion. Finally, the parent prints the shared variable.

What it proves:
Because standard isolated processes cannot modify each other's global variables, a successful modification of the counter (from 0 to 100) proves that true shared-memory concurrency (a lightweight process) was successfully achieved by the kernel.














xv6 kernel is booting

hart 1 starting
hart 2 starting
init: starting sh
$ threadtest
Parent: Shared counter is initially: 0
Thread: Changing the shared counter to 100...
Parent: Thread joined. Shared counter is now: 100
$   