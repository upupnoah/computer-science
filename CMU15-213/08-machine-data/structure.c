#include <stdlib.h>

struct rec {
    int a[4];
    size_t i;
    struct rec *next;
};

void set_i(struct rec *r, size_t val)
{
    r->i = val;
}

int *get_ap(struct rec *r, size_t idx)
{
    return &r->a[idx];
}

int check_val(struct rec *r, int val) {
    int found = 0;
    while (!found && r) {
	size_t i = r->i;
	if (r->a[i] == val)
	    found = 1;
	else
	    r = r->next;
    }
    return found;
}

void set_val(struct rec *r, int val) {
    while (r) {
	int i = r->i;
	r->a[i] = val;
	r = r->next;
    }
}

