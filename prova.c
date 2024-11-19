
#include <stdio.h>
#include <stdlib.h>

unsigned int gcd(unsigned int u, unsigned int v) {
    int shift, uz, vz;
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
        v = abs(diff);
    } while (1);
    return u << shift;
}


int main()
{
    int N = 10;

    int m = 2, n = 1;
    int count = 0;
    int m_squared = 4;
    int n_squared = 1;
    while (m_squared < N)
    {
        if (m_squared + n_squared < N && (m & 1) != (n & 1) && gcd(m, n) == 1)
        {
            count++;
        }
        if (n < m-1){
            n_squared = n_squared + (n << 1) + 1;
            n++;
        }
        else{
            m++;
            m_squared = m_squared + (m << 1) - 1;
            n = 1;
            n_squared = 1;
        }
    }
    printf("%d\n", count);
}
