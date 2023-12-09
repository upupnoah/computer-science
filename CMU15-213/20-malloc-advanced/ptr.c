/* 
 *   prt.c - illustrates some subtleties of C operator precedence and
 *           associativity. The goal is to decrement the integer that p
 *           points to and not p itself.
 */
#include <stdio.h>

int main() 
{
    int foo=100;
    int bar=200;
    int baz=300;
    int *p = &baz;
    
    printf("&foo=%p &bar=%p %baz=%p\n", &foo, &bar, &baz);
    
    /* This is the behavior we want: decrement baz. */
    (*p)--;
    printf("(*p)-- baz=%d *p=%d p=%p\n", baz, *p, p);

    /* Without parens, p is first decremented by four, and then
       dereferenced, with no impact on baz. Equivalent to *(p--).
       This happens because -- and * have the same precedence, but
       associate right to left */
    *p--;
    printf("*p--   baz=%d *p=%d p=%p\n", baz, *p, p);

    /* Equivalent to *p--. Pre-increment and post-increment are the
       same because p isn't used (read) */
    *--p;
    printf("*--p   baz=%d *p=%d p=%p\n", baz, *p, p);

    return(0);
}
