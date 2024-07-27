#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

int main() {
    FILE *test_file = fopen("token", "r");

    if (test_file == NULL) {
        perror("fopen");
        return 1;
    }

    int del = 0;
    char com[128];

    while (fscanf(test_file, "%d %s", &del, com) == 2) {
        pid_t p = fork();
        if (p < 0) {
            perror("fork");
            return 1;
        }

        if (!p) {
            sleep(del);
            int ret = execlp(com, com, NULL);
            perror("execlp");
            return -1;
        }
    }

    fclose(test_file);
    return 0;
}
