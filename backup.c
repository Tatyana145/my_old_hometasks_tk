#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <string.h>
#include <sys/wait.h>

const int n = 256;

void backup(char* source, char* stock){
    DIR* dir_sourse = opendir(source)
    DIR* dir_stock = opendir(stock);

    if (!dir_sourse || !dir_stock) {
        perror("cannot open directory");
        return;
    }

    struct dirent* cur_dir_sourse;
    while(cur_dir_sourse = readdir(dir_sourse)){
        if (!strcmp(cur_dir_sourse->d_name, ".") || !strcmp(cur_dir_sourse->d_name, "..")) continue;

        char new_source[n];
        strcpy(new_source, source);
        strcat(new_source, "/");
        strcat(new_source, cur_dir_sourse->d_name);

        char new_stock[n];
        strcpy(new_stock, stock);
        strcat(new_stock, "/");
        strcat(new_stock, cur_dir_sourse->d_name);

        struct stat info_source;
        struct stat info_stock;

        if(stat(new_source, &info_source) == -1) perror("stat");

        if (S_ISDIR(info_source.st_mode)) {
            if(stat(new_stock, &info_stock) == -1) mkdir(new_stock, 0777);
            backup(new_source, new_stock);
        } else if (S_ISREG(info_source.st_mode)){
            char stock[n];
            strcpy(stock, new_stock);
            strcat(stock, "/");
            strcat(stock, ".gz");

            if (stat(stock, info_stock) == -1) perror("stat");

            if (info_stock.st_mtime < info_source.st_mtime) {
                if (fork() == 0) {
                    execlp("cp", "cp", stock, new_source, NULL);
                    perror("copy");
                    return;
                } else if(fork() == -1) {
                    perror("fork");
                    return;
                }

                wait(NULL);

                if (fork() == 0){
                    execlp("gzip", "gzip", "-f", stock, NULL)
                    perror("gzip");
                    return;
                } else if(fork() == -1) {
                    perror("fork");
                    return;
                }

                wait(NULL);
            }
        }
    }

    closedir(dir_sourse);
    closedir(dir_stock);
    return;
}

int main(int argc, char* argv[], char* envp[]){
    if (argc < 3) {
        perror("incorrect number of arguments");
        return 1;
    }

    backup(argv[1], argv[2]);
    return 0;
}
