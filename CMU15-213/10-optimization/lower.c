#include <stdlib.h>

/* Sample implementation of library function strlen */
/* Compute length of string */
size_t strlen(const char *s)
{
    size_t length = 0;
    while (*s != '\0') {
	s++; 
	length++;
    }
    return length;
}

/* Convert string to lowercase: slow */
void lower1(char *s)
{
    size_t i;

    for (i = 0; i < strlen(s); i++)
	if (s[i] >= 'A' && s[i] <= 'Z')
	    s[i] -= ('A' - 'a');
}

/* Convert string to lowercase: faster */
void lower2(char *s)
{
    size_t i;
    size_t len = strlen(s);

    for (i = 0; i < len; i++)
	if (s[i] >= 'A' && s[i] <= 'Z')
	    s[i] -= ('A' - 'a');
}


