#include "csapp.h"

void sigint_handler(int sig) /* SIGINT handler */
{
    printf("So you think you can stop the bomb with ctrl-c, do you?\n");
    sleep(2);
    printf("Well..."); 
    fflush(stdout);
    sleep(1);
    printf("OK. :-)\n");
    exit(0);                   
}                              

int main() 
{
    /* Install the SIGINT handler */         
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
	unix_error("signal error");               
    
    /* Wait for the receipt of a signal */
    pause(); 
    
    return 0;
}
