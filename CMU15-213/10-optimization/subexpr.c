long sum_neighbors(long *val, long i, long j, long n) {
  long up, down, left, right, sum;
  /* Sum neighbors of i,j */
  up =    val[(i-1)*n + j  ];
  down =  val[(i+1)*n + j  ];
  left =  val[i*n     + j-1];
  right = val[i*n     + j+1];
  sum = up + down + left + right;
  return sum;
}

long sum_neighbors_opt(long *val, long i, long j, long n) {
  long up, down, left, right, sum;
  /* Sum neighbors of i,j */
  long inj = i*n + j;
  up =    val[inj - n];
  down =  val[inj + n];
  left =  val[inj - 1];
  right = val[inj + 1];
  sum = up + down + left + right;
  return sum;
}
