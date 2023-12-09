/* Demonstration of buffer overflow */

#include <stdio.h>
#include <stdlib.h>

/* Implementation of library function gets() */
char *gets(char *dest)
{
  int c = getchar();
  char *p = dest;
  while (c != EOF && c != '\n') {
    *p++ = c;
    c = getchar();
  }
  *p = '\0';
  return dest;
}

/* Read input line and write it back */
void echo()
{
    char buf[4];  /* Way too small! */
    gets(buf);
    puts(buf);
}

void call_echo() {
    echo();
}

void smash() {
    printf("I've been smashed!\n");
    exit(0);
}

int main()
{
    printf("Type a string:");
    call_echo();
    return 0;
}
