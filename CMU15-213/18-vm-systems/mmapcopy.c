/* $begin mmapcopy */
#include "csapp.h"

/* 
 * mmapcopy - uses mmap to copy 
 *            file fd to stdout 
 */
void mmapcopy(int fd, int size) 
{

    /* Ptr to memory mapped VM area */
    char *bufp; 

    bufp = Mmap(NULL, size, PROT_READ, 
		MAP_PRIVATE, fd, 0);
    Write(1, bufp, size);
    return;
}

/* mmapcopy driver */
int main(int argc, char **argv) 
{
    struct stat stat;
    int fd;

    /* Check for required cmd line arg */
    if (argc != 2) {
	printf("usage: %s <filename>\n", 
	       argv[0]);
	exit(0);
    }

    /* Copy input file to stdout */
    fd = Open(argv[1], O_RDONLY, 0);
    Fstat(fd, &stat);
    mmapcopy(fd, stat.st_size);
    exit(0);
}
/* $end mmapcopy */

