#include <stdio.h>

int main(int argc, char const *argv[]) {
    char name[128];
    unsigned int age;

    printf("Hello! It's me, printf.\n");

    printf("What is your name?: ");
    fgets(name, sizeof name, stdin);

    printf("What is your age?: ");
    fscanf(stdin, "%u", &age);

    printf("Hello, %s (%u)!\n", name, age);

    return 0;
}
