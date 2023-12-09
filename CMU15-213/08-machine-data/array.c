#include <stdlib.h>

#define N 16

typedef int fix_matrix[N][N];

#define IDX(n, i, j) ((i)*(n)+(j))

/* Get element a[i][j] */
int fix_ele(fix_matrix a, size_t i, size_t j) {
  return a[i][j];
}

/* Get element a[i][j] */
int var_ele(size_t n, int a[n][n], size_t i, size_t j) {
  return a[i][j];
}

/* Get element a[i][j] */
int vec_ele(size_t n, int *a, size_t i, size_t j) {
  return a[IDX(n,i,j)];
}

/* Retrieve column j from array */
void fix_column(fix_matrix a, size_t j, int *dest)
{
  size_t i;
  for (i = 0; i < N; i++)
    dest[i] = a[i][j];
}

/* Retrieve column j from array */
void vec_column(size_t n, int *a, size_t j, int *dest)
{
  size_t i;
  for (i = 0; i < n; i++)
    dest[i] = a[IDX(n,i,j)];
}

/* Retrieve column j from array */
void var_column(size_t n, size_t a[n][n], size_t j, int *dest)
{
  size_t i;
  for (i = 0; i < n; i++)
    dest[i] = a[i][j];
}

/* Retrieve column j from array */
void fix_column_p(fix_matrix a, size_t j, int *dest)
{
    int *ap = &a[0][j];
    int *aend = &a[N][j];
    do {
	*dest = *ap;
	dest++;
	ap += N;
    } while (ap != aend);
}

void var_column_p(size_t n, int a[n][n], size_t j, int *dest)
{
    int	*ap = &a[0][j];
    int *dend = &dest[n];
    while (dest != dend) {
	*dest = *ap;
	dest++;
	ap += n;
    }
}
