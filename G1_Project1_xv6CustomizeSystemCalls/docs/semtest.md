xv6 kernel is booting

hart 2 starting
hart 1 starting
init: starting sh
$ semtest
Starting Semaphore Test...
Parent: CWhiaitldi:n I agm f door ichnigld s otmo e heavy fwiniosh.r
k...
Child: Work finished. Sending signal to parent!
Parent: I received the signal. Child is done. Exiting.
$ 

X.X sem_create(), sem_down(), and sem_up() --> Inter-Process Communication (Semaphores)
The sem_create(), sem_down(), and sem_up() system calls are implemented in the xv6 operating system to provide robust Inter-Process Communication (IPC) and synchronization. Because user processes in xv6 are strictly isolated, these calls utilize a fixed array of counting semaphore structures allocated securely within kernel memory to act as a shared state. sem_create() allocates and initializes a semaphore, while sem_down() (Wait/P) and sem_up() (Signal/V) allow processes to atomically decrement or increment the counter. Crucially, these calls utilize the kernel's internal sleep and wakeup mechanisms to completely pause waiting processes, ensuring high CPU efficiency by avoiding busy-wait loops. These system calls demonstrate how isolated user programs can safely coordinate their execution timelines. They are invoked from user programs and executed in the kernel through functions sys_sem_create(), sys_sem_down(), and sys_sem_up().

X.X semtest --> Semaphore Synchronization Verification
The semtest user program is designed to verify the correct implementation of the semaphore system calls. It explicitly tests whether a parent process can successfully suspend its own execution and wait for a signal from an isolated child process.

How it works:
The program initializes a global kernel semaphore with a value of 0 and then fork()s a child process. The parent immediately invokes sem_down(), forcing the kernel to put it to sleep because the required resource count is zero. Meanwhile, the child process simulates a heavy workload using a CPU busy-loop. Once finished, the child calls sem_up(), incrementing the semaphore and triggering the kernel to wake the parent back up to finish its execution.

What it proves:
The successful, ordered output of the program—where the parent explicitly waits and only resumes exiting after the child signals completion—proves that the kernel's process synchronization, sleep/wakeup state management, and memory-isolated communication are functioning flawlessly.