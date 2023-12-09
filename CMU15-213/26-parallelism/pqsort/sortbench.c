#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "pqsort.h"
#include "realtimer.h"

#define MAXCPY 5

static data_t *data[MAXCPY] = {NULL};

static void gen_data(int ncpy, size_t nele) {
  int c;
  if (ncpy > MAXCPY) {
    printf("Error.  Cannot have more than %d copies of data.  Exiting\n", MAXCPY);
    exit(0);
  }
  for (c = 0; c < ncpy; c++) {
    data[c] = (data_t *) calloc(nele, sizeof(data_t));
    if (!data[c]) {
      printf("Error.  Couldn't generate array of size %lu.  Exiting\n", (printi_t) nele);
      exit(0);
    }
  }
  size_t i;
  for (i = 0; i < nele; i++) {
    data_t v = random(); 
    for (c = 0; c < ncpy; c++)
      data[c][i] = v; 
  }
}

static void free_data() {
  int c;
  for (c = 0; c < MAXCPY; c++) {
    if (data[c]) {
      free((void *) data[c]);
      data[c] = NULL;
    }
  }
}

/* Test sort function.  Optionally compare results to library version.
   If comp_ptr non-NULL, set it to the number of comparisons
   Return number of seconds */
static double run_test(sort_fun_t sfun, size_t nele, int check,
		       size_t *comp_ptr) {
  /* Generate data */
  double t;
  int ncpy = check ? 3 : 2;
  gen_data(ncpy, nele);
  if (verbose >= 2) {
    printf("Initial data:");
    show_data(data[0], nele);
  }
  start_timer();
  comp_cnt = 0;
  sfun(data[0], nele, data[1]);
  t = elapsed_time();
  if (verbose >= 2) {
    printf("Sorted data:");
    show_data(data[0], nele);
  }
  if (comp_ptr)
    *comp_ptr = comp_cnt;
  if (check) {
    size_t i;
    qsort_lib(data[2], nele, data[1]);
    for (i = 0; i < nele; i++) {
      if (data[0][i] != data[2][i]) {
	printf("Sort error.  Element %lu/%lu.  Library value = %llu.  Sort value = %llu.\n",
	       (printi_t) i, (printi_t) nele, (printd_t) data[2][i], (printd_t) data[0][i]);
	exit(0);
      }
      if (i < nele-1 && data[0][i] > data[0][i+1]) {
	printf("Sort error.  Element %lu = %llu.  Element %lu = %llu.\n",
	       (printi_t) i, (printd_t) data[0][i], (printi_t) i+1, (printd_t) data[0][i+1]);
	exit(0);
      }
    }
  }
  free_data();
  return t;
}

static void usage(char *cmdname) {
  printf("Usage: %s [-hlc] [-n nele] [-v verb] [-t tlim] [-f sfrac] [-f pfrac]\n", cmdname);
  printf("\t-h\tPrint this message\n");
  printf("\t-n nele\tSet number of elements\n");
  printf("\t-v verb\tSet verbosity level\n");
  printf("\t-t tlim\tSet limit on number of concurrent threads\n");
  printf("\t-f frac\tFraction of total when start doing sequential sort\n");
  printf("\t-F frac\tFraction of total when start doing sequential partition\n");
  printf("\t-l\tUse library qsort for serial sort\n");
  printf("\t-c\tCheck result against library qsort\n");
  exit(0);
}

int main(int argc, char *argv[]) {
  size_t nele = 1UL << 20;
  size_t comps = 0;
  int c;
  int check = 0;
  while ((c = getopt(argc, argv, "hn:v:f:F:lc")) != -1) {
    switch(c) {
    case 'h': usage(argv[0]);
      break;
    case 'n':
      nele = strtoul(optarg, NULL, 0);
      break;
    case 'v':
      verbose = atoi(optarg);
      break;
    case 'f':
      serial_sort_fraction = strtoul(optarg, NULL, 0);
      break;
    case 'F':
      serial_partition_fraction = strtoul(optarg, NULL, 0);
      break;
    case 'l':
      use_qsort_lib = 1;
      break;
    case 'c':
      check = 1;
      break;
    default:
      printf("Invalid option '%c'\n", c);
      usage(argv[0]);
    }
  }
  double t = run_test(tqsort, nele, check, &comps);
  printf("%.2f seconds\n", t);
#ifdef LOGCOMPS
  printf("%lu comparisons\n", comps);
#endif
  return 0;
}
