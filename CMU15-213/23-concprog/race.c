/* Test of race condition in thread argument processing */
#include "csapp.h"

/*
  The idea here is to create a very sparse table of buckets.
  Each thread will generate an index using a random number generator.
  It will then deposit its copy of the variable "i" in the table
  at the bucket specified by the index.
  We want to have enough buckets and a good enough randomizer that
  the probability of two threads using the same bucket is very low.
*/


/* How many threads will be created */

#define CNT 100

/* How many buckets do we need */
#define POP 50000

/* The table */
int ivals[POP] = {-1};
/* A count. For each value of "i", how many threads have that value */
int ihist[CNT] = {0};
/* A distribution of how many repetitions there are of each value of i */
int ireps[CNT] = {0};

/* The cycle counter is a hardware register that gets incremented every
   clock cycle.  We'll use it to generate index values, so that each thread
   will (with high probability) get a different value.
*/

/* Assembly code to read cycle counter */
void access_counter(unsigned *hi, unsigned *lo)
{
  /* Get cycle counter */
  asm("rdtsc; movl %%edx,%0; movl %%eax,%1" 
      : "=r" (*hi), "=r" (*lo)
      : /* No input */ 
      : "%edx", "%eax");
}

/* return number between 0 & POP-1 based on cycle counter */
int sample_index() {
  unsigned hi, lo;
  access_counter(&hi, &lo);
  return lo % POP;
}

static void save_value(int i) {
    int s = sample_index();
    ivals[s] = i;
}


/* thread routine */
void *thread(void *vargp) 
{  
    Pthread_detach(pthread_self());
    int i = *((int *)vargp);
    save_value(i);
    return NULL;
}

int main(int argc, char *argv[]) {
    int i, r, maxr, s;
    pthread_t tid;
    int scount = 0;
    for (i = 0; i < CNT; i++) {
      Pthread_create(&tid, NULL, thread, &i);
    }
    sleep(5);
    for (s = 0; s < POP; s++) {
      if (ivals[s] > 0) {
	scount++;
#if 0
	printf("Sample %d, index %d\n", s, ivals[s]);
#endif
	ihist[ivals[s]]++;
      }
    }
    maxr = -1;
    printf("Value\tCount\n");
    for (i = 0; i < CNT; i++) {
      r = ihist[i];
      ireps[r]++;
      if (r > maxr)
	maxr = r;
      printf("%d\t%d\n", i, ihist[i]);
    }
    printf("%d samples detected\n", scount);
    
    for (r = 0; r <= maxr; r++) {
      printf("%d\t%d\n", r, ireps[r]);
    }

    return 0;
}
