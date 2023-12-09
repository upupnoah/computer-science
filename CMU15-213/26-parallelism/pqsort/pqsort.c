/* Implementation of parallel quicksort */

#include "csapp.h"
#include "taskq.h"
#include "pqsort.h"

/* Tuning parameters */
/* Verbosity level */
int verbose = 0;
/* Use library quicksort or homebrewed one? */
int use_qsort_lib = 0;
/* How small do partitions need to be relative to original size
   before using sequential sort & sequential partititoning */
size_t serial_sort_fraction = 32;
size_t serial_partition_fraction = 1;

/* Counting number of comparisons */
volatile size_t comp_cnt;
pthread_mutex_t count_mutex;

/* Debugging support */

#define SHOW_NELE_MAX 30
void show_data(data_t *base, size_t nele) {
    size_t i;
    for (i = 0; i < nele && i < SHOW_NELE_MAX; i++) {
	printf(" %llu", (printd_t) base[i]);
    }
    printf("\n");
}


/* For tracking debugging information */
static data_t *global_base = NULL;
static data_t *global_scratch_base = NULL;
static data_t *global_end = NULL;
static data_t *global_scratch_end = NULL;

size_t global_index(data_t *p) {
    if (p >= global_base && p <= global_end) 
	return p - global_base;
    if (p >= global_scratch_base && p <= global_scratch_end) 
	return p - global_scratch_base;
    return (size_t) -1;  
}

#if 0
ls #define CHK(p) check_range(p)
#else
#define CHK(p) p
#endif

#if LOGCOMPS
#define LTE(x,y) less_equal_than(x,y)
#else
#define LTE(x,y) ((x) <= (y))
#endif

/* Make sure pointer is within range of data */
static data_t *check_range(data_t *p) {
    if (p >= global_base && p <= global_end)
	return p;
    if (p >= global_scratch_base && p <= global_scratch_end)
	return p;
    printf("Error: Attempt to access element %ld or %ld\n",
	   (long int) (p - global_base),
	   (long int) (p - global_scratch_base));
    exit(1);
}

/* Compare two numbers and increment count */
static int less_equal_than(data_t x, data_t y) {
#ifdef LOGCOMPS
    pthread_mutex_lock(&count_mutex);
#endif  
    comp_cnt++;
#ifdef LOGCOMPS
    pthread_mutex_unlock(&count_mutex);
#endif
    return x <= y;
}

/*
  Partition size when start doing sequential sort
  (Computed at beginning)
*/
static size_t nele_max_sort_serial = (1UL << 24);
/*
  Partition size when start doing sequential sort
  (Computed at beginning)
*/
static size_t nele_max_partition_serial = (1UL << 24);

/* Comparision function for qsort */
int dcomp(const void *p1, const void *p2) {
    data_t x1 = *CHK((data_t *) p1);
    data_t x2 = *CHK((data_t *) p2);
    if (!LTE(x2,x1))
	return -1;
    if (x1 == x2)
	return 0;
    return 1;
}

/* Using library quicksort */
void qsort_lib(data_t *base, size_t nele, data_t *scratch_base) {
    qsort((void *) base, nele, sizeof(data_t), dcomp);
}

/* Pick pivot for quicksort */
static data_t *pivot(data_t *base, size_t nele) {
    return base + nele/2;
}

static void swap(data_t *p1, data_t *p2) {
    data_t x1 = *CHK(p1);  data_t x2 = *CHK(p2);
    *p1 = x2; *p2 = x1;
}

/* Partition array into elements <= and >= to pivot.
   Return index of pivot element.
*/
static size_t partition_serial(data_t *base, size_t nele);
static size_t partition_parallel(data_t *base, size_t nele,
				 data_t *scratch_base, size_t block_size);


static size_t partition_sweep(data_t *base, size_t nele, data_t pivot);

static size_t partition(data_t *base, size_t nele, data_t *scratch_base)
{
    if (nele <= nele_max_partition_serial)
	return partition_serial(base, nele);
    else
	return partition_parallel(base, nele, scratch_base, nele_max_partition_serial);
}

static size_t partition_serial(data_t *base, size_t nele) {
    /* Select pivot */
    data_t *p = pivot(base, nele);
    /* Move to right of array */
    data_t pval = *CHK(p);
    swap(p, base+nele-1);
    /* Do partititoning sweep */
    size_t pindex = partition_sweep(base, nele-1, pval);
    /* Move pivot to be first element of right partition */
    swap(base + pindex, base+nele-1);
    if (verbose >= 3) {
	size_t l = global_index(base);
	size_t m = global_index(base+pindex);
	size_t r = global_index(base + nele - 1);
	printf("Serial partition.  [%lu,%lu,%lu]\n",
	       (printi_t) l, (printi_t) m, (printi_t) r);
    }
    return pindex;
}

/* Partitioning sweep.
   Partitions into left (all elements <= p) and right (all elements >= p).
   Returns count of number of elements in left partition.
*/
size_t partition_sweep(data_t *base, size_t nele, data_t pval)
{
    data_t *left = base;
    data_t *right = base+nele-1;
    while (1) {
	while (left <= right && LTE(*CHK(left), pval)) {
	    left++;
	}
	while (left <= right && LTE(pval, *CHK(right))) {
	    right--;
	}
	if (left < right) {
	    swap(left, right);
	    left++;
	    right--;
	} else
	    break;
    }
    return left - base;
}

/* Parallel copy.  Needed as part of parallel partitioning */
/* Have structure that defines copying task */
typedef struct {
    data_t *src;
    data_t *dest;
    size_t nele;
} copy_task_t, *copy_task_ptr;

void *new_copy_task(data_t *src, data_t *dest, size_t nele) {
    copy_task_ptr p = (copy_task_ptr) Malloc(sizeof(copy_task_t));
    p->src = src;
    p->dest = dest;
    p->nele = nele;
    return (void *) p;
}

/* Copying thread function */
void *copy_thread(void *vargp) {
    copy_task_ptr p = (copy_task_ptr) vargp;
    data_t *src = p->src;
    data_t *dest = p->dest;
    size_t nele = p->nele;
    free(vargp);
    if (verbose >= 3) {
	printf("Copy thread.  %lu elements from %lu to %lu\n",
	       (printi_t) nele,
	       (printi_t) global_index(src),
	       (printi_t) global_index(dest));
    }
    bcopy((void *) src, (void *) dest, nele * sizeof(data_t));
    return NULL;
}

void dcopy_parallel(data_t *src, data_t *dest, size_t nele, size_t block_size) {
    int nblock = (nele+block_size-1) / block_size;
    int npb = nele / nblock;
    task_queue_ptr tq = new_task_queue();
    int b;
    if (verbose >= 2) {
	printf("Dcopy.  Copying %lu elements from %lu to %lu in %d blocks\n",
	       (printi_t) nele, (printi_t) global_index(src),
	       (printi_t) global_index(dest), nblock);
    }
    for (b = 0; b < nblock; b++) {
	data_t *bsrc = src + b*npb;
	data_t *bdest = dest + b*npb;
	size_t bnele = (b < nblock-1) ? npb : nele - b*npb;
	void *p = new_copy_task(bsrc, bdest, bnele);
	spawn_task(tq, copy_thread, p);
    }
    join_tasks(tq);
    free_task_queue(tq);
}

/* Parallel partitioning
   1. Copy (in parallel) to scratch space 
   2. Do partitioning
   a. Choose global pivot p.
   b. Break into k blocks and serially partition each block by p.
   c. Copy left subblocks into left hand side 
   & right subblocks into right hand side
*/

/* All information needed for parallel partitioning */
typedef struct {
    data_t *base;
    data_t *scratch_base;      /* Extra space for setting up partitions */
    size_t next_left_index;    /* Next position for accumulating left subblocks */
    size_t next_right_index;   /* Next position for accumulating subblocks */
    pthread_mutex_t mutex;     /* Mutex for shared info */
    data_t pval;               /* Pivot value */
} partition_t;

void init_partition(partition_t *par, data_t *base, size_t nele,
		    data_t *scratch_base, data_t pval)
{
    par->base = base;
    par->scratch_base = scratch_base;
    par->next_left_index = 0;
    par->next_right_index = nele;
    pthread_mutex_init(&par->mutex, NULL);
    par->pval = pval;
}

/* Information stored in each entry task queue for partitioning */
typedef struct {
    size_t nele;
    size_t src_index;
    partition_t *par;
} partition_task_t;

void *new_partition_task(size_t nele, size_t src_index, partition_t *par)
{
    partition_task_t *pt = Malloc(sizeof(partition_task_t));
    pt->nele = nele;
    pt->src_index = src_index;
    pt->par = par;
    return (void *) pt;
}

/* Thread routine for partitioning */
static void *partition_thread(void *vargp) {
    partition_task_t *pt = (partition_task_t *) vargp;
    size_t nele = pt->nele;
    size_t src_index = pt->src_index;
    partition_t *par = pt->par;
    free(vargp);
    /* Do sequential partitioning */
    size_t nleft = partition_sweep(par->scratch_base + src_index, nele,
				   par->pval);
    size_t nright = nele - nleft;
    /* Get destination positions */
    pthread_mutex_lock(&par->mutex);
    size_t left_index = par->next_left_index;
    par->next_left_index += nleft;
    par->next_right_index -= nright;
    size_t right_index = par->next_right_index;
    pthread_mutex_unlock(&par->mutex);
    if (verbose >= 2) {
	size_t glsrc = global_index(par->scratch_base + src_index);
	size_t gldest = global_index(par->base + left_index);
	size_t grsrc = global_index(par->scratch_base + src_index + nleft);
	size_t grdest = global_index(par->base + right_index);
	printf("Partition thread.  %lu elements\n", (printi_t) nele);
	printf("\tMoving left subblock (%lu elements) from sb[%lu] to b[%lu]\n",
	       (printi_t) nleft, (printi_t) glsrc, (printi_t) gldest);
	printf("\tMoving right subblock (%lu elements) from sb[%lu] to s[%lu]\n",
	       (printi_t) nright, (printi_t) grsrc, (printi_t) grdest);
    }
    bcopy((void *) (par->scratch_base + src_index),
	  (void *) (par->base + left_index),
	  nleft * sizeof(data_t)); 
    bcopy((void *) (par->scratch_base + src_index + nleft),
	  (void *) (par->base + right_index),
	  nright * sizeof(data_t));
    return NULL;
}

static size_t partition_parallel(data_t *base, size_t nele,
				 data_t *scratch_base, size_t block_size)
{
    /* Select pivot */
    data_t *p = pivot(base, nele);
    data_t pval = *CHK(p);
    /* Move to right of array */
    swap(p, base+nele-1);
    /* Copy everything else into scratch area */
    dcopy_parallel(base, scratch_base, nele-1, block_size);
    /* Now run multiple partition tasks */
    task_queue_ptr tq = new_task_queue();
    partition_t partition;
    init_partition(&partition, base, nele-1, scratch_base, pval);
    int nblock = (nele + block_size - 2) / block_size;
    size_t npb = (nele-1) / nblock;
    int b;
    for (b = 0; b < nblock; b++) {
	size_t src_index = b*npb;
	size_t bnele = (b < nblock-1) ? npb : nele-1 - b*npb;
	void *p = new_partition_task(bnele, src_index, &partition);
	spawn_task(tq, partition_thread, p);
    }
    join_tasks(tq);
    size_t left_size = partition.next_left_index;
    free_task_queue(tq);
    /* Swap pivot back */
    swap(base+left_size, base+nele-1);
    if (verbose >= 2) {
	printf("Parallel partitioning.  Pivot = %lu. [%lu,%lu,%lu] in %d blocks\n",
	       (printi_t) pval,
	       (printi_t) global_index(base),
	       (printi_t) global_index(base+left_size),
	       (printi_t) global_index(base+nele-1),
	       nblock);
    }
    if (verbose >= 3) {
	printf("Partitioned results: ");
	show_data(base, nele);
    }
    return left_size;
}

void qsort_serial(data_t *base, size_t nele,
		  data_t *scratch_base) {
    if (verbose >= 3) {
	size_t l = global_index(base);
	size_t r = global_index(base + nele - 1);
	printf("Qsort_serial.  [%lu,%lu]\n", (printi_t) l, (printi_t) r);
    }
    if (nele <= 1)
	return;
    if (nele == 2) {
	if (base[0] > base[1])
	    swap(base, base+1);
	return;
    }
    size_t m = partition_serial(base, nele);
    if (m > 1)
	qsort_serial(base, m, scratch_base);
    if (nele-1 > m+1)
	qsort_serial(base+m+1, nele-m-1, scratch_base+m+1);
}

/* For threaded sorting, have structure that defines sorting task */
typedef struct {
    data_t *base;
    size_t nele;
    data_t *scratch_base;
    task_queue_ptr tq;
} sort_task_t, *sort_task_ptr;

static void init_task(size_t nele) {
    if (serial_sort_fraction > 1) {
	nele_max_sort_serial = nele / serial_sort_fraction;
	if (nele_max_sort_serial < 2)
	    nele_max_sort_serial = 2;
    }
    else
	nele_max_sort_serial = nele;
    if (verbose >= 1) {
	printf("Serial sort cutover = %lu\n", (printi_t) nele_max_sort_serial);
    }
    if (serial_partition_fraction > 1) {
	nele_max_partition_serial = nele / serial_partition_fraction;
	if (nele_max_partition_serial < 2)
	    nele_max_partition_serial = 2;
    }
    else
	nele_max_partition_serial = nele;
    if (verbose >= 1) {
	printf("Serial partition cutover = %lu\n", (printi_t) nele_max_partition_serial);
    }
#ifdef LOGCOMPS
    pthread_mutex_init(&count_mutex, NULL);
#endif  
}

/* Add a new task to the list.  Returns position in task list */
static sort_task_ptr new_task(data_t *base, size_t nele,
			      data_t *scratch_base, task_queue_ptr tq) {
    sort_task_ptr t = Malloc(sizeof(sort_task_t));
    t->base = base;
    t->nele = nele;
    t->scratch_base = scratch_base;
    t->tq = tq;
    if (verbose >= 3) {
	size_t l = global_index(base);
	size_t r = global_index(base + nele-1);
	printf("New_task. [%lu,%lu]\n", (printi_t) l, (printi_t) r);
    }
    return t;
}

static void tqsort_helper(data_t *base, size_t nele,
			  data_t *scratch_base, task_queue_ptr tq);

/* Thread routine for many-threaded quicksort */
static void *sort_thread(void *vargp) {
    sort_task_t *t = (sort_task_t *) vargp;
    data_t *base = t->base;
    size_t nele = t->nele;
    data_t *scratch_base = t->scratch_base;
    task_queue_ptr tq = t->tq;
    free(vargp);
    if (verbose >= 2) {
	size_t l = global_index(base);
	size_t r = global_index(base + nele-1);
	printf("sort_thread.  tid=0x%lx  [%lu,%lu]\n",
	       (printi_t) pthread_self(), (printi_t) l, (printi_t) r);
    }
    size_t m = partition(base, nele, scratch_base);
    if (m > 1)
	tqsort_helper(base, m, scratch_base, tq);
    if (nele-1 > m+1)
	tqsort_helper(base+m+1, nele-m-1, scratch_base+m+1, tq);
    return NULL;
}

/* Multi-threaded quicksort */
static void tqsort_helper(data_t *base, size_t nele,
			  data_t *scratch_base, task_queue_ptr tq) {
    if (verbose >= 2) {
	size_t l = global_index(base);
	size_t r = global_index(base+nele-1);
	printf("tqsort.  [%lu,%lu]\n", (printi_t) l, (printi_t) r);
    }
    if (nele <= nele_max_sort_serial) {
	/* Use sequential sort */
	if (use_qsort_lib)
	    qsort_lib(base, nele, scratch_base);
	else
	    qsort_serial(base, nele, scratch_base);
	return;
    }
    sort_task_t *t = new_task(base, nele, scratch_base, tq);
    spawn_task(tq, sort_thread, (void *) t);
}

void tqsort(data_t *base, size_t nele, data_t *scratch_base) {
    init_task(nele);
    global_base = base;
    global_end = global_base + nele - 1;
    global_scratch_base = scratch_base;
    global_scratch_end = global_scratch_base + nele - 1;
    task_queue_ptr tq = new_task_queue();
    tqsort_helper(base, nele, scratch_base, tq);
    int max_tasks = join_tasks(tq);
    free_task_queue(tq);
    if (verbose >= 1)
	printf("Sorted %lu elements with max %d concurrent tasks\n",
	       (printi_t) nele, max_tasks);
}


