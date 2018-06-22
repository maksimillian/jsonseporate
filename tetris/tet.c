#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define c_width 12
#define c_heigth 21

enum { f_i = 1, f_o, f_t, f_s, f_z, f_j, f_l };

typedef struct crds {
    int x, y;
} coords;

typedef struct figure figure_t;
typedef struct cup cup_t;

struct cup {
    int f[c_width][c_heigth];
};

cup_t cup;

struct figure {
    int a : 16;
    int p : 2;
    coords st, nd, rd, th;
    void (*print)(figure_t);
    void (*rotate_cw)(figure_t);
    void (*rotate_ccw)(figure_t);
    // int (*width)(figure_t);
    // int (*height)(figure_t);
    int (*mayFall)(figure_t);
};

void printCoords(coords c1, coords c2, coords c3, coords c4) {
    cup.f[c1.x][c1.y] = 2;
    cup.f[c2.x][c2.y] = 2;
    cup.f[c3.x][c3.y] = 2;
    cup.f[c4.x][c4.y] = 2;
}

void printFig(figure_t f) {
    cup.f[f.st.x][f.st.y] = 2;
    cup.f[f.nd.x][f.nd.y] = 2;
    cup.f[f.rd.x][f.rd.y] = 2;
    cup.f[f.th.x][f.th.y] = 2;
}

void setFig(figure_t f, int n) {
    cup.f[f.st.x][f.st.y] = n;
    cup.f[f.nd.x][f.nd.y] = n;
    cup.f[f.rd.x][f.rd.y] = n;
    cup.f[f.th.x][f.th.y] = n;
}

int mayFall(figure_t f) {
    if ((cup.f[f.st.x][f.st.y + 1] == 1 || cup.f[f.st.x][f.st.y + 1] == 3) ||
        (cup.f[f.nd.x][f.nd.y + 1] == 1 || cup.f[f.nd.x][f.nd.y + 1] == 3) ||
        (cup.f[f.rd.x][f.rd.y + 1] == 1 || cup.f[f.rd.x][f.rd.y + 1] == 3) ||
        (cup.f[f.th.x][f.th.y + 1] == 1 || cup.f[f.th.x][f.th.y + 1] == 3)) {
        return 0;
    }

    return 1;
}

void rotate_cw(figure_t f) {
    int arr[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    arr[f.st.x % 3 - 1][f.st.y % 3 - 1] = 1;
    arr[f.nd.x % 3 - 1][f.nd.y % 3 - 1] = 2;
    arr[f.rd.x % 3 - 1][f.rd.y % 3 - 1] = 3;
    arr[f.th.x % 3 - 1][f.th.y % 3 - 1] = 4;
    for (int x = 0; x < 3 / 2; x++) {
        for (int y = x; y < 3 - x - 1; y++) {
            int tmp = arr[x][y];
            arr[x][y] = arr[y][3 - 1 - x];
            arr[y][3 - 1 - x] = arr[3 - 1 - x][3 - 1 - y];
            arr[3 - 1 - x][3 - 1 - y] = arr[3 - 1 - y][x];
            arr[3 - 1 - y][x] = tmp;
        }
    }
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (arr[x][y] == 1) {
                f.st.x -= f.st.x % 3 - x;
                f.st.y -= f.st.y % 3 - y;
            } else if (arr[x][y] == 2) {
                f.nd.x -= f.nd.x % 3 - x;
                f.nd.y -= f.nd.y % 3 - y;
            } else if (arr[x][y] == 3) {
                f.rd.x -= f.rd.x % 3 - x;
                f.rd.y -= f.rd.y % 3 - y;
            } else if (arr[x][y] == 4) {
                f.th.x -= f.th.x % 3 - x;
                f.th.y -= f.th.y % 3 - y;
            }
        }
    }
}

void f_i_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_i_rotate_cw(figure_t f) {
    setFig(f, 0);
    f.st.x += 2;
    f.st.y -= 1;
    f.nd.x += 1;
    f.nd.y += 0;
    f.rd.x = 0;
    f.rd.y += 1;
    f.th.x -= 1;
    f.th.y += 2;
    setFig(f, 2);
}
void f_i_rotate_ccw(figure_t f) {}
int f_i_mayFall(figure_t f) { return 1; }

void f_o_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_o_rotate_cw(figure_t f) {}
void f_o_rotate_ccw(figure_t f) {}
int f_o_mayFall(figure_t f) { return 1; }

void f_t_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_t_rotate_cw(figure_t f) {}
void f_t_rotate_ccw(figure_t f) {}
int f_t_mayFall(figure_t f) { return 1; }

void f_s_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_s_rotate_cw(figure_t f) {}
void f_s_rotate_ccw(figure_t f) {}
int f_s_mayFall(figure_t f) { return 1; }

void f_z_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_z_rotate_cw(figure_t f) {}
void f_z_rotate_ccw(figure_t f) {}
int f_z_mayFall(figure_t f) { return 1; }

void f_j_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_j_rotate_cw(figure_t f) {}
void f_j_rotate_ccw(figure_t f) {}
int f_j_mayFall(figure_t f) { return 1; }

void f_l_print(figure_t f) { printCoords(f.st, f.nd, f.rd, f.th); }
void f_l_rotate_cw(figure_t f) {}
void f_l_rotate_ccw(figure_t f) {}
int f_l_mayFall(figure_t f) { return 1; }

void spawnFigure(figure_t *g) {
    figure_t f = *g;
    f.a = (int)(7 * ((double)random() / (double)RAND_MAX)) + 1;
    f.p = 0;
    switch (f.a) {
    case f_i:
        f.print = &printFig;
        f.rotate_cw = &rotate_cw;
        f.rotate_ccw = &f_i_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 4;
        f.st.y = 0;
        f.nd.x = 5;
        f.nd.y = 0;
        f.rd.x = 6;
        f.rd.y = 0;
        f.th.x = 7;
        f.th.y = 0;
        break;
    case f_o:
        f.print = &printFig;
        f.rotate_cw = &f_o_rotate_cw;
        f.rotate_ccw = &f_o_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 5;
        f.st.y = 0;
        f.nd.x = 6;
        f.nd.y = 0;
        f.rd.x = 6;
        f.rd.y = 1;
        f.th.x = 5;
        f.th.y = 1;
        break;
    case f_t:
        f.print = &printFig;
        f.rotate_cw = &rotate_cw;
        f.rotate_ccw = &f_t_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 5;
        f.st.y = 0;
        f.nd.x = 4;
        f.nd.y = 1;
        f.rd.x = 5;
        f.rd.y = 1;
        f.th.x = 6;
        f.th.y = 1;
        break;
    case f_s:
        f.print = &printFig;
        f.rotate_cw = &f_s_rotate_cw;
        f.rotate_ccw = &f_s_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 4;
        f.st.y = 1;
        f.nd.x = 5;
        f.nd.y = 1;
        f.rd.x = 5;
        f.rd.y = 0;
        f.th.x = 6;
        f.th.y = 0;
        break;
    case f_z:
        f.print = &printFig;
        f.rotate_cw = &f_z_rotate_cw;
        f.rotate_ccw = &f_z_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 4;
        f.st.y = 0;
        f.nd.x = 5;
        f.nd.y = 0;
        f.rd.x = 5;
        f.rd.y = 1;
        f.th.x = 6;
        f.th.y = 1;
        break;
    case f_j:
        f.print = &printFig;
        f.rotate_cw = &f_j_rotate_cw;
        f.rotate_ccw = &f_j_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 4;
        f.st.y = 0;
        f.nd.x = 4;
        f.nd.y = 1;
        f.rd.x = 5;
        f.rd.y = 1;
        f.th.x = 6;
        f.th.y = 1;
        break;
    case f_l:
        f.print = &printFig;
        f.rotate_cw = &f_l_rotate_cw;
        f.rotate_ccw = &f_l_rotate_ccw;
        f.mayFall = &mayFall;
        f.st.x = 4;
        f.st.y = 1;
        f.nd.x = 5;
        f.nd.y = 1;
        f.rd.x = 6;
        f.rd.y = 1;
        f.th.x = 6;
        f.th.y = 0;
        break;
    }
    *g = f;
}

void initCup() {

    for (int y = 0; y < c_heigth; y++) {
        for (int x = 0; x < c_width; x++) {
            if (x == 0 || x == c_width - 1 || y == c_heigth - 1)
                cup.f[x][y] = 1;
            else
                cup.f[x][y] = 0;
        }
    }
}

void printCup() {
    char s = '\n';
    system("clear");
    for (int y = 0; y < c_heigth; y++) {
        for (int x = 0; x < c_width; x++) {
            if (cup.f[x][y] == 1) {
                write(1, "\u2FFF", 3);
                write(1, " ", 1);
            } else if (cup.f[x][y] == 2 || cup.f[x][y] == 3) {
                write(1, "\u2FFF", 3);
                write(1, " ", 1);
            } else {
                write(1, "\u2121", 1);
                write(1, " ", 1);
            }
        }
        write(1, &s, 1);
    }
}

void step(figure_t *g, coords d) {
    figure_t f = *g;
    if (mayFall(f)) {
        setFig(f, 0);
        f.st.x += d.x;
        f.st.y += (d.y + 1);
        f.nd.x += d.x;
        f.nd.y += (d.y + 1);
        f.rd.x += d.x;
        f.rd.y += (d.y + 1);
        f.th.x += d.x;
        f.th.y += (d.y + 1);
        f.print(f);
        printCup();
    } else {
        setFig(f, 3);
        spawnFigure(&f);
    }
    *g = f;
}

int main(int argc, char const *argv[]) {
    struct termios t;
    char c;

    char block[] = {0xE7, 0x16, 0xAA};
    char *s;
    coords d;

    figure_t fig;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(0, TCSANOW, &t);
    int flags = fcntl(0, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
    // FD_SET(0, &f);
    initCup();
    spawnFigure(&fig);
    fig.print(fig);
    printCup();
    // fcntl(1, O_NONBLOCK);

    int pool = epoll_create1(0);
    // IF...

    struct epoll_event ev = {
        .events = EPOLLIN
        //, .data = {.fd = 0, /**/ .ptr = NULL, .u32 = 0, .u64 = 0 }
    };
    epoll_ctl(pool, EPOLL_CTL_ADD, 0, &ev);
    // IF...

    while (1) {

        d.x = 0;
        d.y = 0;
        c = 0;

        int n = epoll_wait(pool, &ev, 1, 500);

        if (n == 0) {
            step(&fig, d);
        } else if (read(0, &c, 1) < 1) {
            // ....
            printf("zhopa\n");
        } else {
            switch (c) {
            case 'q':
                break;
            case 'e':
                fig.rotate_cw(fig);
                break;
            case 'a':
                if (fig.st.x - 1 > 0 && fig.nd.x - 1 > 0 && fig.rd.x - 1 > 0 &&
                    fig.th.x - 1 > 0)
                    d.x = -1;
                d.y = 0;
                break;
            case 's':
                d.x = 0;
                d.y = 0;
                break;
            case 'd':
                if (fig.st.x + 1 < c_width - 1 && fig.nd.x + 1 < c_width - 1 &&
                    fig.rd.x + 1 < c_width - 1 && fig.th.x + 1 < c_width - 1)
                    d.x = 1;
                d.y = 0;
                break;
            }
            step(&fig, d);
            s = "\033[38;5;206m\u25A3 ";
            // write(1, s, 14);
            //  printCup();
        }
    }
    t.c_lflag &= ICANON;
    t.c_lflag &= (ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(0, TCSANOW, &t);
    return 0;
}
