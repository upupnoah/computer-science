int gt (long x, long y) {
  return x > y;
}

int zero(long x) {
    return x == 0L;
}


long absdiff(long x, long y)
{
    long result;
    if (x > y)
	result = x-y;
    else
	result = y-x;
    return result;
}

long absdiff_j(long x, long y)
{
    long result;
    int ntest = x <= y;
    if (ntest) goto Else;
    result = x-y;
    goto Done;
 Else:
    result = y-x;
 Done:
    return result;
}


long absdiff_cm(long x, long y)
{
    long result;
    result = x-y;
    long eval = y-x;
    if (x <= y)
	result = eval;
    return result;
}
