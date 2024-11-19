#include <stdio.h>
#include <string.h>
#include <arm_neon.h>

char gcd_cache[1024*1024] __attribute__((aligned(64)));
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
  unsigned int n_end,m,n;

  for (m = 1; (n_end=m*m) < N; m++) {
    n_end = N - n_end;
    char* __restrict__ p = &gcd_cache[m*1024 + m + 1];

    for (n=m+1; (n+126)*(n+126) <= n_end; n += 128) {
      // Prefetch data
      __builtin_prefetch(p + 128, 0, 3);
      __builtin_prefetch(p + 144, 0, 3);
      __builtin_prefetch(p + 160, 0, 3);
      __builtin_prefetch(p + 176, 0, 3);
      __builtin_prefetch(p + 192, 0, 3);
      __builtin_prefetch(p + 208, 0, 3);
      __builtin_prefetch(p + 224, 0, 3);
      __builtin_prefetch(p + 240, 0, 3);

      uint8x16_t data[8];
      for (int i = 0; i < 8; i++) data[i] = vld1q_u8((uint8_t*)(p + i * 16));
            
      uint8x16_t sum1 = vaddq_u8(data[0], data[1]);
      uint8x16_t sum2 = vaddq_u8(data[2], data[3]);
      uint8x16_t sum3 = vaddq_u8(data[4], data[5]);
      uint8x16_t sum4 = vaddq_u8(data[6], data[7]);

      uint8x16_t sum = vaddq_u8(vaddq_u8(sum1, sum2), vaddq_u8(sum3, sum4));

      uint8x8_t vec_low = vget_low_u8(sum);
      uint8x8_t vec_high = vget_high_u8(sum);
      uint8x8_t reduced = vpadd_u8(vec_low, vec_high);
      reduced = vpadd_u8(reduced, reduced);
      reduced = vpadd_u8(reduced, reduced);
      reduced = vpadd_u8(reduced, reduced);

      count += vget_lane_u8(reduced, 0);
      
      p += 128;
    }

    for (; (n+30)*(n+30) <= n_end; n+=32) {
      __builtin_prefetch(p + 64, 0, 3);
      uint8x16_t data1 = vld1q_u8((uint8_t*)p);
      uint8x16_t data2 = vld1q_u8((uint8_t*)(p+16));

      uint8x16_t sum = vaddq_u8(data1, data2);
      uint8x8_t vec_low = vget_low_u8(sum);
      uint8x8_t vec_high = vget_high_u8(sum);
      uint8x8_t reduced = vpadd_u8(vec_low, vec_high);
      reduced = vpadd_u8(reduced, reduced);
      reduced = vpadd_u8(reduced, reduced);
      reduced = vpadd_u8(reduced, reduced);

      count += vget_lane_u8(reduced, 0);
      p+=32;
    }

    //epilog
    unsigned int n_start = n*n;
    for(; n_start <= n_end; n +=2) {
      count += gcd_cache[m*1024 + n];
      n_start += ((n << 2) + 4);
    }
  }
  return count;
}

int main(int argc, char* argv[]) {
	unsigned int N, sol;

  int n_start;

  for (int m = 1; m < 1001; m++) {
    for (int n = m+1; n < 1001; n+=2) gcd_cache[m*1024 + n] = gcd(m,n);
  }

  memset(n_cache, 0, sizeof(n_cache));

  n_cache[1000000] = 159139;

	while (scanf("%u", &N), N) {
    sol = n_cache[N] ? n_cache[N] : (n_cache[N] = compute_triplets(N));
		printf("%u\n", sol);
	}

	return 0;
}
