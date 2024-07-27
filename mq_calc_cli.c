#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <stdio.h>

#include <unistd.h>

struct Task {
    long m_type; // == 1

    pid_t src;
    float a, b;
    char op; // '+', '-', '*', '/'
};

struct Ans {
    long m_type; // == Task.src

    float res;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s A {+-*/} B\n", argv[0]);
        return 1;
    }

    struct Task task;
    task.m_type = 1;
    task.src = getpid();
    task.a = atof(argv[1]);
    task.op = argv[2][0];
    task.b = atof(argv[3]);

    key_t k = ftok("token", 0);
    if (k < 0) {
        perror("ftok");
        return 1;
    }

    int mq = msgget(k, 0);
    if (mq < 0) {
        perror("msgget");
        return 2;
    }

    if (msgsnd(mq, &task, sizeof(struct Task) - sizeof(long), 0) < 0) {
        perror("msgsnd");
        return 3;
    }

    printf("Task sent: %f %c %f\n", task.a, task.op, task.b);

    struct Ans ans;
    if (msgrcv(mq, &ans, sizeof(struct Ans) - sizeof(long), task.src, 0) < 0) {
        perror("msgrcv");
        return 4;
    }

    printf("Answer recieved: %f\n", ans.res);

    return 0;
}
