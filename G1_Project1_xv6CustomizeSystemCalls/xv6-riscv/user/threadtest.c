#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Global shared variable
int shared_counter = 0;

void thread_function() {
    printf("Thread: Changing the shared counter to 100...\n");
    shared_counter = 100;
    exit(0);
}

int main(int argc, char *argv[]) {
    printf("Parent: Shared counter is initially: %d\n", shared_counter);

    // Allocate 4096 bytes (1 page) for the thread's stack
    void *stack = malloc(4096); 
    
    // Stack grows downwards in memory! So we must pass the TOP of the stack
    int thread_id = clone((void*)((uint64)stack + 4096));

    if(thread_id == 0) {
        // Child thread jumps to the function
        thread_function();
    } else {
        // Parent waits for the thread to finish
        join();
        printf("Parent: Thread joined. Shared counter is now: %d\n", shared_counter);
    }
    
    exit(0);
}