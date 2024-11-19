#include <stdio.h>
#include <string.h>
#include <arm_neon.h>

char gcd_cache[1024][1024] __attribute__((aligned(64)));
unsigned int n_cache[1000001] __attribute__((aligned(64)));

// Bithacks version of euclidian gcd
// Seen in https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
// Version con cambios para tener solo 0s o 1s, que interesa para el problema
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
  unsigned int n_start,n_end,m,n;

  for (m = 1; (n_end=m*m) < N; m++) {
    n_end = N - n_end;
    char* p = &gcd_cache[m][m+1];
    for (n=m+1; (n+62)*(n+62) <= n_end; n += 64) {
      uint8x16_t data1 = vld1q_u8((uint8_t*)p);
      uint8x16_t data2 = vld1q_u8((uint8_t*)(p+16));
      uint8x16_t data3 = vld1q_u8((uint8_t*)(p+32));
      uint8x16_t data4 = vld1q_u8((uint8_t*)(p+48));
      uint8x16_t sum = vaddq_u8(data1, data2);
      uint8x16_t sum2 = vaddq_u8(data3, data4);
      sum = vaddq_u8(sum, sum2);
      uint8_t sum_array[16];
      vst1q_u8(sum_array, sum);
      for (int i = 0; i < 16; i++) count += sum_array[i];
      p+=64;
    }
    //epilog
    for(; n*n <= n_end; n +=2) count += gcd_cache[m][n];
  }
  return count;
}

int main(int argc, char* argv[]) {
	unsigned int N, sol;

  int n_start;

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
