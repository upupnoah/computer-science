#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int input_arg;


void handler(int sig) {
    printf("%d: Received signal %d\n", input_arg, sig);
    fflush(stdout);
    exit(0);
}


int main(int argc, char **argv) {
    volatile int val, *p;

    signal(SIGSEGV, handler);

    if (argc != 2) {
	printf("Usage: %s <k>\n", argv[0]);
	exit(1);
    }
    input_arg = atoi(argv[1]);

    switch (input_arg) {
    case 0: 
	/* Page exists, but not allowed to read from kernel space */
	printf("Page exists, but not allowed to read from kernel area\n");
	p = (int *)0x800000000000; /* first byte above stack */

	val = *p;
	break;

    case 1:
	/* Try to read from a page that does not exist */
	printf("Try to read from page that does not exist\n");
	p = (int *) 0;
	val = *p;
	break;

    case 2:
	/* Page exists, but not allowed to write to text area */
	printf("Page exists, but not allowed to write to text area\n");
	p = (int *) main;
	val = 0;
	*p = val;
	break;

    case 3:
	/* Try to read from a page that exists with read permission */
	printf("Try to read from page that does exist\n");
	p = (int *) main;
	val = *p;
	break;


    default:
	printf("Usage: %s <k>\n", argv[0]);
	exit(1);

    }

    printf("%d: OK\n", input_arg);
    exit(0);
}
