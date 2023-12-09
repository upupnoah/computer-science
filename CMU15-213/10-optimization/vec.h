/* Sample data type */
typedef double data_t;

/* Create abstract data type for vector */
typedef struct {
    size_t len;
    data_t *data;
} vec;

/* Create vector */
vec *new_vec(size_t len);

/* Free storage used by vector */
void free_vec(vec *v);

/*
 * Retrieve vector element and store in dest.
 * Return 0 (out of bounds) or 1 (successful)
 */
int get_vec_element(vec *v, size_t index, data_t *dest);

/* Macro version */
#define GET_VEC_ELEMENT(v,index,dest) \
    !((index) < 0 || (index) >= (v)->len) && \
    *(dest) = (v)->data[(index)], 1;


data_t *get_vec_start(vec *v);

/*
 * Set vector element.
 * Return 0 (out of bounds) or 1 (successful)
 */

int set_vec_element(vec *v, size_t index, data_t val);

/* Get vector length */
size_t vec_length(vec *v);

/* Set length of vector.  If > allocated length, will reallocate */
void set_vec_length(vec *v, size_t newlen);
