#include <sys/types.h>
#include <unistd>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handler(int sig) {
    printf("Hello! Signal #%d\n", sig);
}

int main() {
    signal(SIGCHILD, handler);

    pid_t p = fork();
    if (p == 0) {
        printf("Child: group %d\n", getpgrp());
        setpgrp();
        printf("Child: group %d\n", getpgrp());
    }

    while (1) {
        sleep(1);
    }

    return 0;
}
