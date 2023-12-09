#include <sys/time.h>

static double start_time;
static struct timezone zone; 

void start_timer()
{
  zone.tz_minuteswest = 0;
  zone.tz_dsttime = 0;
  struct timeval time;
  gettimeofday(&time, &zone);
  start_time = (double) time.tv_sec + 1e-6*time.tv_usec;
}

double elapsed_time() {
  struct timeval time;
  gettimeofday(&time, &zone);
  double new_time = (double) time.tv_sec + 1e-6*time.tv_usec;
  return new_time - start_time;
}
