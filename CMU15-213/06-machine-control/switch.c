#include <stdio.h>
#include <stdlib.h>

long switch_eg
(long x, long y, long z) {
  long w = 1;
  switch(x) {
  case 1:
    w = y*z;
    break;
  case 2:
    w = y/z;
    /* Fall through */
  case 3:
    w += z;
    break;
  case 5:
  case 6:
    w -= z;
    break;
  default:
    w = 2;
  }
  return w;
}

long sparse_switch_eg
(long x, long y, long z) {
  long w = 1;
  switch(x) {
  case 100:
    w = y*z;
    break;
  case 200:
    w = y/z;
    /* Fall through */
  case 300:
    w += z;
    break;
  case 500:
  case 600:
    w -= z;
    break;
  default:
    w = 2;
  }
  return w;
}

int main(int argc, char *argv[]) {
  long x = atoi(argv[1]);
  long y = atoi(argv[2]);
  long z = atoi(argv[3]);
  long w = switch_eg(x, y, z);
  printf("x = %ld, y = %ld, z = %ld --> %ld\n",
	 x, y, z, w);
  exit(0);
}
