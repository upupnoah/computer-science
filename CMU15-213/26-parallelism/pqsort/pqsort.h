/* Sort routines */

/* Data type */
typedef long unsigned data_t;
/* Index printing type */
typedef long unsigned printi_t;
/* Data printing type */
typedef long long unsigned printd_t;

/* Sorting function type. */
typedef void (*sort_fun_t)(data_t *base, size_t nele, data_t *scratch_base);

/* Tuning parameters */
/* Verbosity level */
extern int verbose;;
/* How small do partitions need to be relative to original size
   before using sequential sort & sequential partititoning */
extern size_t serial_sort_fraction;
extern size_t serial_partition_fraction;
/* For sequential sort: use library quicksort or homebrewed one? */
extern int use_qsort_lib;
/* Counter that gets incremented for every comparison */
extern volatile size_t comp_cnt;

/* Debugging helpers */
void show_data(data_t *base, size_t nele);

/* Based on library quicksort */
void qsort_lib(data_t *base, size_t nele, data_t *scratch_base);

/* Serial quicksort */
void qsort_serial(data_t *base, size_t nele, data_t *scratch_base);

/* Many-threaded quicksort */
void tqsort(data_t *base, size_t nele, data_t *scratch_base);
