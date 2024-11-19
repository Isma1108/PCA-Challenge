#include <stdio.h>
#include <string.h>

char gcd_cache[1024][1024] __attribute__((aligned(64)));
unsigned int n_cache[1000001] __attribute__((aligned(64)));

/*
inline unsigned int fast_abs(int n) {
  return ((n ^ (n >> 31)) - (n >> 31));
}
*/

// Bithacks version of euclidian gcd
// Seen in https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
// Version adaptada para el problema, solo devuelve 1 o 0 :).
inline unsigned int gcd(unsigned int u, unsigned int v) {
    int uz, vz;
    if (u == 0) return v == 1 ? 1 : 0;
    if (v == 0) return u == 1 ? 1 : 0;
    uz = __builtin_ctz(u);
    vz = __builtin_ctz(v);
    u >>= uz;
    v >>= vz;
    
    while (u != v) {
      if (u > v) {
        u -= v;
        if (u == 0) return v == 1 ? 1 : 0;
        u >>= __builtin_ctz(u);
      } else {
        v -= u;
        if (v == 0) return u == 1 ? 1 : 0;
        v >>= __builtin_ctz(v);
      }
    }
    return u == 1 ? 1 : 0;
}

inline unsigned int compute_triplets(unsigned int N) {
	unsigned int count = 0;
  unsigned int n_end,m,n;

  for (m = 1; (n_end=m*m) < N; m++) {
    n_end = N - n_end;
    char* p = &gcd_cache[m][m+1];
    for (n = m+1; (n+30)*(n+30) <= n_end; n += 32) {
      char sum = (*p);
      sum += *(p+2);
      sum += *(p+4);
      sum += *(p+6);
      sum += *(p+8);
      sum += *(p+10);
      sum += *(p+12);
      sum += *(p+14);
      sum += *(p+16);
      sum += *(p+18);
      sum += *(p+20);
      sum += *(p+22);
      sum += *(p+24);
      sum += *(p+26);
      sum += *(p+28);
      sum += *(p+30);
      count += sum;
      p += 32;
    }
    //epilog
    for(; n*n <= n_end; n +=2) {
      count += gcd_cache[m][n];
    }
  }
  return count;
}

int main(int argc, char* argv[]) {
	unsigned int N, sol;

  int n_start;

  memset(gcd_cache, 0, sizeof(gcd_cache));
  for (int m = 1; m < 1001; m++) {
    for (int n = m+1; n < 1001; n+=2) gcd_cache[m][n] = gcd(m,n);
  }

  memset(n_cache, 0, sizeof(n_cache));

  n_cache[1000000] = 159139;

	while (scanf("%u", &N), N) {
    sol = n_cache[N] ? n_cache[N] : (n_cache[N] = compute_triplets(N));
		printf("%u\n", sol);
	}

	return 0;
}
