#include <stdio.h>

int main() {
    char buf[4];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = 'd';
    printf("%s\n", buf);
    return 0;
}