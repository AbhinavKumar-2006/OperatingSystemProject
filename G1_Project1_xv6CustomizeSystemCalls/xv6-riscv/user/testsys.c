#include "kernel/types.h"
#include "user/user.h"

int main() {

    setpriority(5);
    printf("Priority: %d\n", getpriority());

    int pid = fork();

    if(pid == 0){
        int m = recv();
        printf("Child got: %d\n", m);
    } else {
        send(pid, 42);
        wait(0);
    }

    signal(getpid());

    exit(0);
}
