#include <math.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    const int steps = 128;
    const double w = 1.0 / steps;
    double x;

    for (x = 0; x < 1; x += w) {
        printf("%.5e\tsin %.5f\tcos %.5g\tln %.5a\n", x, sin(x), cos(x),
               log(x));
    }

    return 0;
}
