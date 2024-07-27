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
    char* text = (argc > 1) ? argv[1] : "Hello world!";

    key_t k = ftok("token", 0);
    if (k < 0) {
        perror("ftok");
        return 1;
    }

    int mq = msgget(k, IPC_CREAT|0600);
    if (mq < 0) {
        perror("msgget");
        return 2;
    }

    //struct Msg msg = {1, "Hello world!"};
    struct Msg msg; msg.m_type = 1;
    //msg.text = "Hello world!"; // <- ERROR
    strcpy(msg.text, text);
   //strncpy(msg.text, text, sizeof(msg.text));

    if (msgsnd(mq, &msg, strlen(msg.text)+1, 0) < 0) {
        perror("msgsnd");
        return 3;
    }

    printf("Message has been sent\n");

    return 0;
}
