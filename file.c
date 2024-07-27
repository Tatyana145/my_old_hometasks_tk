#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

enum {ERR_IO = 1, ERR_LOGIC = 2, ERR_OK = 0};
//char fn[] = "myfile.txt";
char fn[] = "mypipe";

int main(int arg, char* argv[]) {
    int _fd[2];
    if(pipe(_fd) < 0) {
        perror("pipe");
        return ERR_IO;
    }

    int f = _fd[1];
    int ff = _fd[0];

    /*int f = open(fn, O_RDWR|O_CREAT, 0640);
    if (f < 0) {
        perror("open");
        return ERR_IO;
    }
    
    printf("file opened. Descriptor: %d\n", f);*/

    printf("Pipe opened for writing. Descriptor: %d %d\n", f, ff);
    
    //---------------------------

    pid_t p = fork();
    if (p > 0) {
        char msg[] = "Hello, world!";
        int len = strlen(msg);
        write(f, msg, len);

        printf("Parent %d write data\n", petpid());
    }
    
    //close(f);
    
    //---------------------------
    
    /*int ff = open(fn, O_RDONLY);
    if (ff < 0) {
        perror("second open");
        return ERR_IO;
    }*/
    
    else if (p == 0) {
        close(f);

        char buf[128] = {'a'};
        int buflen = 128;
        read(ff, buf, buflen);

        printf("Child %d read %d byres\n", len);
        printf("Data: %s\n", buf);
    }
    
    return ERR_OK;
}
