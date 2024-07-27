#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <stdio.h>

struct Msg {
    long m_type; // should be 1
    char text[128];
};

int main(int argc, char* argv[]) {
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

    //struct Msg msg = {1, "Hello world!"};
    struct Msg msg;
    //msg.text = "Hello world!"; // <- ERROR
    strcpy(msg.text, "Hello world!");

    int sz = msgrcv(mq, &msg, sizeof(struct Msg) - sizeof(long), 1, 0);
    if (sz < 0) {
        perror("msgrcv");
        return 3;
    }

    printf("Message has been recieved: %s\n", msg.text);

    return 0;
}
