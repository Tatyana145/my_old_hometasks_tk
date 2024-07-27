#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

int listing(char* dirname) {
    printf("---::: %s :::---\n", dirname);

    DIR* d = opendir(dirname);
    if (!d) {
        perror("opendir");
        return 1;
    }

    struct dirent* ent = NULL;
    while((ent = readdir(d)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
            continue;
        }

        char fname[FILENAME_MAX];
        sprintf(fname, "%s/%s", dirname, ent->d_name);
        //strcat(fname, dirname);
        //strcat(fname, "/");
        //strcat(fname, ent->d_name);

        struct stat ifo;
        if (lstat(fname, &ifo) < 0) {
            perror("stat");
            continue;
        }

        mode_t m = ifo.st_mode;
        char* stype = NULL;
        if (S_ISREG(m)) {
            stype = "regular";
        } else if (S_ISDIR(m)) {
            stype = "dir";
            listing(fname);
        } else if (S_ISLNK(m)) {
            stype = "symlink";
        } else if (S_ISFIFO(m)) {
            stype = "fifo";
        }

        switch(type = ifo.st_mode & S_IFMT) {
        case S_IFREG: stype = "reg"; break;
        case S_IFDIR: stype = "dir"; break;
        //
        }

        printf("%10s         %ld        %ld        %s\n", ent->d_name, ent->d_ino, ifo.st_size, stype);
    }

    closedir(d);
    printf("---/\n");
    return 0;
}

int main() {
     return listing(".");
}
