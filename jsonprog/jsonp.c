#include <stdio.h>
#include <stdlib.h>
#define MX 40000000

int w = 0;
char out[MX];

void TABB(int tab) {
    for (int i = 0; i < tab; i++) {
        out[w++] = ' ';
    }
}

int main(int argc, char const *argv[]) {
    FILE *fp = /*stdin;*/ fopen("text.txt", "rb");
    FILE *fout = /*stdout;*/ fopen("out.txt", "wb");
    int tab = 0;
    int c;

    while (!feof(fp)) {
        c = fgetc(fp);
        if ((c == '{') || (c == '[')) {
            out[w++] = c;
            tab += 1;
            out[w++] = '\n';
            TABB(tab);
        } else if ((c == '}') || (c == ']')) {
            out[w++] = '\n';
            tab -= 1;
            TABB(tab);
            out[w++] = c;
        } else if (c == 'n') {
            out[w++] = 'n';
            out[w++] = 'u';
            out[w++] = 'l';
            out[w++] = 'l';
        } else if (c == 't') {
            out[w++] = 't';
            out[w++] = 'r';
            out[w++] = 'u';
            out[w++] = 'e';
        } else if (c == 'f') {
            out[w++] = 'f';
            out[w++] = 'a';
            out[w++] = 'l';
            out[w++] = 's';
            out[w++] = 'e';
        } else if (('0' <= c) && (c <= '9')) {
            do {
                out[w++] = c;
                c = fgetc(fp);
            } while ((('0' <= c) && (c <= '9')));
            if (c == '.') {
                do {
                    out[w++] = c;
                    c = fgetc(fp);
                } while ((('0' <= c) && (c <= '9')));
            }
            ungetc(c, fp);
        } else if (c == '\"') {
            int f = 0;
            do {
                out[w++] = c;
                if (f == 1) {
                    w--;
                }
                f = 0;
                c = fgetc(fp);
                if (c == '\\') {
                    out[w++] = c;
                    c = fgetc(fp);
                    out[w++] = c;
                    c = ' ';
                    f = 1;
                }
            } while (c != '\"');
            out[w++] = c;
        } else if (c == ':') {
            out[w++] = c;
            out[w++] = ' ';
        } else if (c == ',') {
            out[w++] = c;
            out[w++] = '\n';
            TABB(tab);
        }
        // else if ((c != ' ') && (c != '\n')) {
        //        out[w++] = c;
        //    }
    }
    fwrite(out, 1, w, fout);
    fclose(fp);
    fclose(fout);
    return 0;
}
