#include <stdlib.h>

struct S1 {
    char c;
    int i[2];
    double v;
} *p1;

struct S2 {
    double v;
    int i[2];
    char c;
} *p2;

struct S3 {
    short i;
    float v;
    short j;
} *p3;

struct S4 {
    char c;
    int i;
    char d;
} *p4;

struct S5 {
    int i;
    char c;
    char d;
} *p5;

struct S3 a[10];

short get_j(size_t idx) {
    return a[idx].j;
}
