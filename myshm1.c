#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include <stdio.h>

#define FILE_TOKEN "myfile.txt"

int main()
{
    key_t k = ftok(FILE_TOKEN, 0);
    if (k < 0) {
        perror("ftok");
        return 1;
    }

    int shmid = shmget(k, 3*sizeof(int), IPC_CREAT|0600);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    void* mem = shmat(shmid, NULL, 0); //~ melloc();
    if (mem == (void*)-1) {
        perror("shmat");
        return 1;
    }

    int* cc = (int*)mem;

    cc[0] += 1;
    /*for (int i = 0; i < 10000; ++i) {
        cc[0] += 1; // exec count for proc1
        // cc[1]    // exec count for proc2
        cc[2] += 1; // exec count for both (proc1+proc2)
        // *(cc + 1) = 123;
    }*/

    int loc = cc[2];
    loc = loc + 1;
    sleep(2);
    cc[2] = loc;

    printf("Counts: %d %d %d\n", cc[0], cc[1], cc[2]);
    //sleep(19);
    return 0;
}
