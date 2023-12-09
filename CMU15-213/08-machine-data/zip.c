#include <stdio.h>
#include <stdlib.h>

#define ZLEN 5
typedef int zip_dig[ZLEN];

zip_dig cmu = { 1, 5, 2, 1, 3 };
zip_dig mit = { 0, 2, 1, 3, 9 };
zip_dig ucb = { 9, 4, 7, 2, 0 };

int get_digit(zip_dig z, size_t dig)
{
  return z[dig];
}

void zincr(zip_dig z) {
  size_t i;
  for (i = 0; i < ZLEN; i++)
    z[i]++;
}

void zincr_p(zip_dig z) {
  int *zend = z+ZLEN;
  do {
    (*z)++;
    z++;
  } while (z != zend);  
}

#define ISIZE sizeof(int)
void zincr_v(zip_dig z) {
  void *vz = z;
  size_t i = 0;
  do {
    (*((int *) (vz+i)))++;
    i += ISIZE;
  } while (i != ISIZE*ZLEN);
}

#define PCOUNT 4
zip_dig pgh[PCOUNT] = 
  {{1, 5, 2, 0, 6},
   {1, 5, 2, 1, 3 },
   {1, 5, 2, 1, 7 },
   {1, 5, 2, 2, 1 }};

int *get_pgh_zip(size_t index)
{
  return pgh[index];
}

int get_pgh_digit
  (size_t index, size_t dig)
{
  return pgh[index][dig];
}

#define UCOUNT 3
int *univ[UCOUNT] = {mit, cmu, ucb};

int get_univ_digit
  (size_t index, size_t dig)
{
  return univ[index][dig];
}
