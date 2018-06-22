#include <stdio.h>
#include <stdlib.h>
#define MX 4000

int main(int argc, char const *argv[]) {
    char in[MX];
    char out[MX];
    char p1 = 0;
    FILE *fp = stdin;    // fopen("text.txt", "rb");
    FILE *fout = stdout; // fopen("out.txt", "wb");

    while (!feof(fp)) {
        int r = fread(in, 1, sizeof in, fp);
        int w = 0;
        int p = 0;
        int tab = 0;

        for (int i = 0; i < r; i++) {

            if (p > 0) {
                out[w] = in[i];
                p--;
                w++;
            } else if (in[i] <= '\xF0') {
                out[w] = in[i];
                w++;
                tab++;
                p = 3;
            } else if (in[i] <= '\xE0') {
                out[w] = in[i];
                w++;
                tab++;
                p = 2;
            } else if (in[i] <= '\xC0') {
                out[w] = in[i];
                w++;
                tab++;
                p = 1;
            } else {
                out[w] = in[i];
                if (out[w] == '\n') {
                    tab = 0;
                } else {
                    tab++;
                }
                w++;
            }

            if ((p == 0) && (tab >= 12)) {
                out[w] = ' ';
                w++;
                out[w] = '\n';
                w++;
                tab = 0;
            }
        }
        fwrite(out, 1, w, stdout);
    }

    // fclose x2
    return 0;
}
