#include <stdio.h>
#include <stdlib.h>

double dadd(double x, double y) {
    return x + y;
}

float fadd(float x, float y) {
    return x + y;
}

double dincr(double *p, double v) {
    double x = *p;
    *p = x + v;
    return x;
}

double dvprod(size_t n, double a[n], double b[]) {
    size_t i;
    double val = 0.0;
    for (i = 0; i < n; i++)
	val += a[i] * b[i];
    return val;
}

float fvprod(size_t n, float a[n], float b[]) {
    size_t i;
    float val = 0.0;
    for (i = 0; i < n; i++)
	val += a[i] * b[i];
    return val;
}

int gt(float x, float y) {
    return x > y;
}

long magnitude(double x) {
    double ref = 1.0;
    long xmag = 0;
    long refmag = 0;
    /* Find reference */
    while (ref != 0) {
	ref = ref/10.0;
	refmag++;
    }
    /* Do the same for x */
    while (x != 0) {
	x = x/10.0;
	xmag++;
    }
    return xmag - refmag;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
	printf("Usage %s val\n", argv[0]);
	return 0;
    }
    double val = atof(argv[1]);
    long m = magnitude(val);
    printf("Order of %.2f = %ld\n", val, m);
    return 0;
}
