#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
//#include <linux/dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

FILE *fout;
int sk = 0;

void SKOB() {
    for (int i = 0; i < sk; i++) {
        fputc(']', fout);
    }
    sk = 0;
}

#define handle_error(msg)                                                      \
    do {                                                                       \
        perror(msg);                                                           \
        exit(EXIT_FAILURE);                                                    \
    } while (0)

struct linux_dirent {
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[];
};

#define BUF_SIZE 1024

void TREE(int fd) {
    int nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;
    //    while((nread = syscall(...)) > 0) {}
    fputc('[', fout);
    sk++;
    for (;;) {
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        if (nread == -1) {
            handle_error("getdents");
        }
        if (nread == 0) {
            break;
        }
        // printf("--------------- nread=%d ---------------\n", nread);
        // printf("inode#    file type  d_reclen  d_off   d_name\n");
        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *)(buf + bpos);
            fprintf(fout, "%8ld  ", d->d_ino);
            char d_type = *(buf + bpos + d->d_reclen - 1);
            if (d_type == DT_DIR) {
                fputc('[', fout);
                sk++;
                int f = openat(fd, d->d_name, O_RDONLY | O_DIRECTORY);
                if (f == -1) {
                    //    if (f.errno != EACCES) {
                    SKOB();
                    exit(-1);
                    //    }
                } else {
                    TREE(f);
                    close(f);
                }
                fputc(']', fout);
                sk--;
            }
            fprintf(fout, "%4d %10lld  %s\n", d->d_reclen, (long long)d->d_off,
                    d->d_name);
            bpos += d->d_reclen;
        }
    }
    fputc(']', fout);
    sk--;
}

int main(int argc, char const *argv[]) {
    char path[PATH_MAX];
    fout = fopen("text.txt", "wb");
    int fd = open(".", O_RDONLY | O_DIRECTORY);
    if (fd == -1) {

    } else {
        TREE(fd);
        close(fd);
    }
    fclose(fout);
}
