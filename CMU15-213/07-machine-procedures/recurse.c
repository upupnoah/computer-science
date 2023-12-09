/* Recursive popcount */
long pcount_r(unsigned long x) {
    if (x == 0)
	return 0;
    else
	return
	    (x & 1) + pcount_r(x >> 1);
}

