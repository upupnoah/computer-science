#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void show_pointer(void *p, char *descr) {
    //    printf("Pointer for %s at %p\n", descr, p);
    printf("%s\t%p\t%lu\n", descr, p, (unsigned long) p);
}

char big_array[1L<<24];    /*  16 MB */
char huge_array[1L<<31];   /*   2 GB */

int global = 0;

int useless() { return 0; }

int main ()
{
    void *p1, *p2, *p3, *p4;
    int local = 0;
    p1 = malloc(1L << 28);
    p2 = malloc(1L << 8);
    p3 = malloc(1L << 32);
    p4 = malloc(1L << 8);

    show_pointer((void *) big_array, "big array");
    show_pointer((void *) huge_array, "huge array");
    show_pointer((void *) &local, "local");
    show_pointer((void *) &global, "global");
    show_pointer((void *) p1, "p1");
    show_pointer((void *) p2, "p2");
    show_pointer((void *) p3, "p3");
    show_pointer((void *) p4, "p4");
    show_pointer((void *) useless, "useless");
    show_pointer((void *) exit, "exit");
    return 0;
}
