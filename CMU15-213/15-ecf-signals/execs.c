/*
 * execs.c - Examples of exec command
 */
#include "csapp.h"

/*
 * exec0 - Simple execl wrapper example
 * Call once, never return
 */
void exec0() 
{
    if (fork() == 0) {
	execl("/bin/ls", "ls", "-F", (char *) NULL);
	printf("Child: Should never get here!\n");
	exit(1);
    }
    wait(NULL);
    printf("Command completed\n");
}

/*
 * exec1 - Simple execve example
 */ 
#define MAXARGS 16
extern char **environ;
void exec1() 
{
    char *myargv[MAXARGS];
    
    myargv[0] = "/bin/ls";
    myargv[1] = "-F";
    myargv[2] = NULL;

    if (fork() == 0) {
	execve(myargv[0], myargv, environ);
	printf("Child: Should never get here!\n");
	exit(1);
    }
    wait(NULL);
    printf("Command completed\n");
}

int main(int argc, char *argv[])
{
    int option = 0;
    if (argc > 1)
	option = atoi(argv[1]);
    switch(option) {
    case 0: exec0();
	break;
    case 1: exec1();
	break;
    default:
	printf("Unknown option %d\n", option);
	break;
    }
    return 0;
}
