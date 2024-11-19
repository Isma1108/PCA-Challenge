#include <stdio.h>
#include <string.h>

unsigned int gcd_cache[1005][1005] __attribute__((aligned(64)));
unsigned int n_cache[1000001] __attribute__((aligned(64)));

inline unsigned int fast_abs(int n) {
  return ((n ^ (n >> 31)) - (n >> 31));
}

// Bithacks version of euclidian gcd
// Seen in https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
inline unsigned int gcd(unsigned int u, unsigned int v) {
 int shift, uz, vz;
  if (u == 0) return v;
  if (v == 0) return u;
  uz = __builtin_ctz(u);
  vz = __builtin_ctz(v);
  shift = uz > vz ? vz : uz;
  u >>= uz;
  do {
    v >>= vz;
    int diff = v;
    diff -= u;
    vz = __builtin_ctz(diff);
    if (diff == 0) break;
    if (v < u) u = v;
    v = fast_abs(diff);
  } while(1);
  return u<< shift;
}


inline unsigned int compute_triplets(unsigned int N) {
	unsigned int count = 0;
  unsigned int n_start,n_end,m,n;

  for (m = 2; (n_end=m*m) <= N; m++) {
    n_start = (m&1 ? 2 : 1);
    n_end = N - n_end;
    unsigned int nsq = n_start*n_start;
    for (n = n_start; n < m && nsq <= n_end; n += 2) {
      if (gcd_cache[m][n] == 1) count++;
      nsq += ((n << 2) + 4);
    }
  }
  return count;
}

int main(int argc, char* argv[]) {
	unsigned int N, sol;

  int n_start;

  for (int m = 2; m < 1001; m++) {
    n_start = (m&1 ? 2 : 1);
    for (int n = n_start; n < m; n++) gcd_cache[m][n] = gcd(m,n);
  }

  memset(n_cache, 0, sizeof(n_cache));

  n_cache[1000000] = 159139;

	while (scanf("%u", &N), N) {
    sol = n_cache[N] ? n_cache[N] : (n_cache[N] = compute_triplets(N));
		printf("%u\n", sol);
	}

	return 0;
}
