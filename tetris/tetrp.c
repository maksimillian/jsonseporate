#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define maxh 10
#define maxl 15

enum { f_i, f_o, f_t, f_s, f_z, f_j, f_l };

typedef struct crds {
    int x, y;
} xycord;

int ffield[maxh][maxl + 1];
int field[maxh][maxl];

struct figure_definition;

typedef struct cup cup_t;
typedef struct figure_definition fig_def_t;
typedef struct figure_state fig_state_t;
typedef struct figure figure_t;

struct cup {
    int ffield[maxh][maxl + 1];
    int field[maxh][maxl];
};

struct figure_definition {
    int (*const fall_1_row)(fig_state_t, const cup_t);
    void (*const print)(const fig_state_t, cup_t);
    int (*const rotate_cw)(fig_state_t, const cup_t);
    int (*const rotate_ccw)(fig_state_t, cup_t);
    int (*const shift_left)(fig_state_t, cup_t);
    int (*const shift_right)(fig_state_t, cup_t);
};

struct figure_state {
    const fig_def_t *m;
    //...
};

struct figure {
    int a : 16;
    int p : 2;
    xycord ss, nn, rr, tt;
    void (*print)(figure_t);
    void (*rotate_cw)(figure_t);
    void (*rotate_ccw)(figure_t);
    int (*mayFall)(figure_t);
};

int f_i_fall_1_row(fig_state_t, cup_t);
void f_i_print(fig_state_t, cup_t);
int f_i_rotate_cw(fig_state_t, cup_t);
int f_i_rotate_ccw(fig_state_t, cup_t);
int f_i_shift_left(fig_state_t, cup_t);
int f_i_shift_right(fig_state_t, cup_t);

void drawffield() {
    for (int j = 0; j < maxh; j++) {
        for (int i = 0; i < (maxl + 1); i++) {
            write(1, &ffield[j][i], 1);
        };
    };
};

void spawnFig(figure_t *g) {
    figure_t f = *g;
    f.a = (int)(7 * ((double)random() / (double)RAND_MAX)) + 1;
    f.p = 0;
    switch (f.a) {
    case f_i:
        f.print = &printFig;
        f.rotate_cw = &f_i_rotate_cw;
        f.rotate_ccw = &f_i_rotate_ccw;
        f.mayFall = &f_i_mayFall;
        f.ss.x = 4;
        f.ss.y = 0;
        f.nn.x = 5;
        f.nn.y = 0;
        f.rr.x = 6;
        f.rr.y = 0;
        f.tt.x = 7;
        f.tt.y = 0;
        break;
    case f_o:
        f.print = &printFig;
        f.rotate_cw = &f_o_rotate_cw;
        f.rotate_ccw = &f_o_rotate_ccw;
        f.mayFall = &f_o_mayFall;
        f.ss.x = 5;
        f.ss.y = 0;
        f.nn.x = 6;
        f.nn.y = 0;
        f.rr.x = 6;
        f.rr.y = 1;
        f.tt.x = 5;
        f.tt.y = 1;
        break;
    case f_t:
        f.print = &printFig;
        f.rotate_cw = &f_t_rotate_cw;
        f.rotate_ccw = &f_t_rotate_ccw;
        f.mayFall = &f_t_mayFall;
        f.ss.x = 5;
        f.ss.y = 0;
        f.nn.x = 4;
        f.nn.y = 1;
        f.rr.x = 5;
        f.rr.y = 1;
        f.tt.x = 6;
        f.tt.y = 1;
        break;
    case f_s:
        f.print = &printFig;
        f.rotate_cw = &f_s_rotate_cw;
        f.rotate_ccw = &f_s_rotate_ccw;
        f.mayFall = &f_s_mayFall;
        f.ss.x = 4;
        f.ss.y = 1;
        f.nn.x = 5;
        f.nn.y = 1;
        f.rr.x = 5;
        f.rr.y = 0;
        f.tt.x = 6;
        f.tt.y = 0;
        break;
    case f_z:
        f.print = &printFig;
        f.rotate_cw = &f_z_rotate_cw;
        f.rotate_ccw = &f_z_rotate_ccw;
        f.mayFall = &f_z_mayFall;
        f.ss.x = 4;
        f.ss.y = 0;
        f.nn.x = 5;
        f.nn.y = 0;
        f.rr.x = 5;
        f.rr.y = 1;
        f.tt.x = 6;
        f.tt.y = 1;
        break;
    case f_j:
        f.print = &printFig;
        f.rotate_cw = &f_j_rotate_cw;
        f.rotate_ccw = &f_j_rotate_ccw;
        f.mayFall = &f_j_mayFall;
        f.ss.x = 4;
        f.ss.y = 0;
        f.nn.x = 4;
        f.nn.y = 1;
        f.rr.x = 5;
        f.rr.y = 1;
        f.tt.x = 6;
        f.tt.y = 1;
        break;
    case f_l:
        f.print = &printFig;
        f.rotate_cw = &f_l_rotate_cw;
        f.rotate_ccw = &f_l_rotate_ccw;
        f.mayFall = &f_l_mayFall;
        f.ss.x = 4;
        f.ss.y = 1;
        f.nn.x = 5;
        f.nn.y = 1;
        f.rr.x = 6;
        f.rr.y = 1;
        f.tt.x = 6;
        f.tt.y = 0;
        break;
    }
    *g = f;
}
}
;
// clang-format on

typedef struct figure {
    int a : 16;
    int p : 2;
    void *print;
    void *rotate;
    int *width;
    int *height;
    int *mayFall;
} figure_t;

int main(int argc, char const *argv[]) {
    cup_t cc;
    cc.ffield[0][maxl] = '\n';
    for (int i = 1; i < (maxh - 1); i++) {
        cc.ffield[i][0] = '|';
        cc.field[i][0] = 1;
        for (int j = 1; j < (maxl - 2); j++) {
            cc.ffield[i][j] = ' ';
        }
        cc.ffield[i][maxl - 1] = '|';
        cc.field[i][maxl - 1] = 1;
        cc.ffield[i][maxl] = '\n';
    };
    for (int i = 1; i < (maxl - 2); i++) {
        cc.ffield[maxh - 1][i] = '_';
        cc.field[maxh - 1][i] = 1;
    };
    cc.ffield[maxh - 1][0] = '|';
    cc.field[maxh - 1][0] = 1;
    cc.ffield[maxh - 1][maxl - 1] = '|';
    cc.field[maxh - 1][maxl - 1] = 1;
    cc.ffield[maxh - 1][maxl] = '\n';
    drawffield();
    fig_state_t s = {.m = &Figures[f_i]};
    s.m->print(s, cc);
    struct termios t;
    char c;
    fd_set f;
    tcgetattr(0, &t);
    figure_t fig;
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(0, TCSANOW, &t);
    int flags = fcntl(0, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
    spawnFig(&fig);
    // FD_SET(0, &f);

    // fcntl(1, O_NONBLOCK);
    while (1) {
        usleep(10000);
        if (read(0, &c, 1) != -1) {

        } else {
        }
    }
    return 0;
}
