#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>

int cc[3] = {0, 0, 0};

void* proc1(void*) {
    cc[0] += 1;
    cc[2] += 2;
}

void* proc2(void*) {
    for (int i = 0; i < 1000000; ++i) {
        cc[1] += 1;
        cc[2] += 1;
    }

    return NULL;
}


int main()
{
    pthread_t thr1;
    if (pthread_create(&thr1, NULL, proc1, NULL) != 0) {
        printf("pthread_create() failed\n");
        return 1;
    }

    // Second thread as master
    sleep(1);
    proc2(NULL);

    // Wait for first thread
    pthread_join(thr1, NULL);

    printf("Counts: %d %d %d\n", cc[0], cc[1], cc[2]);
    return 0;
}
