#include <stdio.h>

void nest1() {
    int A1[3];
    int *A2;
    printf("A1 -> %d\n", sizeof(A1));
    printf("A2 -> %d\n", sizeof(A2));
    printf("*A1 -> %d\n", sizeof(*A1));
    printf("*A2 -> %d\n", sizeof(*A2));
}

void nest2() {
    int A1[3];
    int *A2[3];
    int (*A3)[3];
    int (*A4[3]);

    printf("A1 -> %d\n", sizeof(A1));
    printf("A2 -> %d\n", sizeof(A2));
    printf("A3 -> %d\n", sizeof(A3));
    printf("A4 -> %d\n", sizeof(A4));
    printf("*A1 -> %d\n", sizeof(*A1));
    printf("*A2 -> %d\n", sizeof(*A2));
    printf("*A3 -> %d\n", sizeof(*A3));
    printf("*A4 -> %d\n", sizeof(*A4));
    //    printf("**A1 -> %d\n", sizeof(**A1));
    printf("**A2 -> %d\n", sizeof(**A2));
    printf("**A3 -> %d\n", sizeof(**A3));
    printf("**A4 -> %d\n", sizeof(**A4));
}


void nest3() {
    int A1[3][5];
    int *A2[3][5];
    int (*A3)[3][5];
    int *(A4[3][5]);
    int (*A5[3])[5];

    printf("A1 -> %d\n", sizeof(A1));
    printf("A2 -> %d\n", sizeof(A2));
    printf("A3 -> %d\n", sizeof(A3));
    printf("A4 -> %d\n", sizeof(A4));
    printf("A5 -> %d\n", sizeof(A5));
    printf("*A1 -> %d\n", sizeof(*A1));
    printf("*A2 -> %d\n", sizeof(*A2));
    printf("*A3 -> %d\n", sizeof(*A3));
    printf("*A4 -> %d\n", sizeof(*A4));
    printf("*A5 -> %d\n", sizeof(*A5));
    printf("**A1 -> %d\n", sizeof(**A1));
    printf("**A2 -> %d\n", sizeof(**A2));
    printf("**A3 -> %d\n", sizeof(**A3));
    printf("**A4 -> %d\n", sizeof(**A4));
    printf("**A5 -> %d\n", sizeof(**A5));
    //    printf("***A1 -> %d\n", sizeof(***A1));
    printf("***A2 -> %d\n", sizeof(***A2));
    printf("***A3 -> %d\n", sizeof(***A3));
    printf("***A4 -> %d\n", sizeof(***A4));
    printf("***A5 -> %d\n", sizeof(***A5));
}


int main() {
    printf("Nest 1:\n");
    nest1();
    printf("Nest 2:\n");
    nest2();
    printf("Nest 3:\n");
    nest3();

    return 0;
}
