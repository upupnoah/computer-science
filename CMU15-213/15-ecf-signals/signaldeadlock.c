#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "csapp.h"
/*
 * This program demonstrates that calling 'printf' (or puts, for that
 * matter, which is what gcc quietly changes the call to) is not safe
 * from within a signal handler that's invoked asynchronously (like
 * SIGCHLD's is).
 * 
 * Start the program, and you'll see it "get stuck" after a while.
 * Attach gdb to it (gdb ./signaldeadlock) "attach <pid>" in gdb,
 * where pid is the process id.
 *
 * @author Godmar Back <godmar@gmail.com>
 * Written for CS 3214 Fall 2009, Virginia Tech
 * 
 * Updated by Dave O'Hallaron to use the sio functions
 * Fall, 2015
 */
#define DEADLOCK 1

#define MAXN 128

static void
catch_child(int signo)
{
#if DEADLOCK
    /* this call may reenter printf/puts! Bad! */
    puts("Child exitted!\n");
#else
    /* This version is async-signal-safe */
    sio_puts("Child exitted!\n");
#endif

    /* reap all children */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
}

int
main()
{
    signal(SIGCHLD, catch_child);

    long i, j;
    for (i = 0; i < 1000000; i++) {
        if (fork() == 0)
            exit(0);

#if DEADLOCK
        printf("Child started\n");
#else
        sio_puts("Child started\n");
#endif

        for (j=0; j<10; j++) {
#if DEADLOCK
            /* Make each printf do a lot of work */
            printf("%ld%ld%ld%ld%ld%ld%ld%ld", i, i, i, i, i, i, i, i);
#else
            sio_emitl("i");
            sio_emitl("i");
            sio_emitl("i");
            sio_emitl("i");
            sio_emitl("i");
            sio_emitl("i");
            sio_emitl("i");
            sio_emitl("i");
#endif
        }
    }
    return 0;
}
