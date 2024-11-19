// Versio inicial per veure si el jutge l'aprova

#include <stdio.h>
#include <string.h>

// Fast gcd using Euclidian algorithm, with recursion 
// O(log(min(a,b)))
//int gcd(int a, int b) {return !b ? a : gcd(b, a%b);}

unsigned int gcd_cache[1005][1005];

inline unsigned int fast_abs(int n) {
  return ((n ^ (n >> 31)) - (n >> 31));
}

// Bithacks version of euclidian gcd
// Seen in https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
inline unsigned int gcd(unsigned int u, unsigned int v) {
  if (gcd_cache[u][v]) return gcd_cache[u][v];
  unsigned int u1 = u, v1 = v;
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
  return gcd_cache[u1][v1] = u << shift;
}


//unsigned int gcd3(unsigned int a, unsigned int b, unsigned int c) {return gcd(gcd(a,b),c);}

inline unsigned int compute_triplets(unsigned int N) {
	unsigned int count = 0;

  for (unsigned int m = 2; m*m <= N; m++) {
    for (unsigned int n = 1; n < m; n++) {
      if ((m-n)&1 && gcd(m,n) == 1) {
        unsigned int x = m * m - n * n;
        unsigned int y = 2 * m * n;
        unsigned int z = m * m + n * n;
        
        if (z <= N) count++;
      }
    }
  }
  return count;
}

int main(int argc, char* argv[]) {
	unsigned int N;

  memset(gcd_cache, 0, sizeof(gcd_cache));

	while (scanf("%u", &N), N) {
		unsigned int sol = compute_triplets(N);
		printf("%u\n", sol);
	}

	return 0;
}