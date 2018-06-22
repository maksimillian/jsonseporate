#define _GNU_SOURCE
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

struct linux_dirent {
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[];
};

#define BUF_SIZE 1024

int TREE(int fd, FILE *fout) {
    int nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;
    int f;
    int coma = 0;
    fputc('[', fout);
    for (;;) {
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        if (nread == -1) {
            f = -1;
            break;
        }
        if (nread == 0) {
            break;
        }

        for (bpos = 0; bpos < nread;
             bpos += d->d_reclen, fputc('}', fout), coma = 1) {

            d = (struct linux_dirent *)(buf + bpos);

            d_type = *(buf + bpos + d->d_reclen - 1);

            // openat, stat
            if (coma == 1) {
                fputc(',', fout);
            };
            /*fprintf(fout, "{\"name\":\"%s\",\"size\":%d,\"d_type\":\"%x\"",
                    d->d_name, -1, d_type); */
            fprintf(fout, "{\"name\":\"%s\"", d->d_name);

            if (d_type == DT_DIR) {
                if (strcmp(d->d_name, ".") == 0)
                    continue;
                if (strcmp(d->d_name, "..") == 0)
                    continue;
                fprintf(fout, "%s", "\"items\":");
                f = openat(fd, d->d_name, O_RDONLY | O_DIRECTORY);
                if (f == -1 && errno != EACCES) {
                    continue;
                } else if (f == -1) {
                    break;
                } else {
                    int r = TREE(f, fout);
                    close(f);
                    f = r;
                }
            }

            if (f == -1) {
                break;
            }
        }
    }
    fputc(']', fout);
    return f == -1 ? -1 : 0;
}

int main(int argc, char const *argv[]) {
    FILE *fout = /*stdout;*/ fopen("text.txt", "wb");
    int fd = open("/home", O_RDONLY | O_DIRECTORY);
    if (fd >= 0) {
        int r = TREE(fd, fout);
        close(fd);
        fd = r;
    }
    fclose(fout);

    if (fd == -1) {
        fprintf(stderr, "%s", strerror(errno));
    }
}
