#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

enum {
    ERR_IO 1,
    ERR_LOGIC 2,
    ERR_OK 0,
};

char fn[] = "myfile.txt";

int main(int arg, char* argv[]) {
    int f = open(fn, O_RDWR|O_CREAT, 0640);
    if (f < 0) {
        perror("open");
        return ERR_IO;
    }
    
    printf("file opened. Descriptor: %d\n", f);
    
    char msg[] = "Hello, world!";
    int len = strlen(msg);
    write(f, data, len+1);
    
    close(f);
    
    //---------------------------
    
    int ff = open(fn, O_RDONLY);
    if (ff < 0) {
        perror("second open");
        return ERR_IO;
    }
    
    char buf[128];
    int buflen = 128;
    read(ff, buf, buflen);
    printf("Data read %d byres\n", len);
    
    return ERR_OK;
}
