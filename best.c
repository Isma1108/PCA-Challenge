#include <stdio.h>
#include <string.h>

#define NMax 1000000
unsigned int n_cache[NMax + 1];

void generate_PPT(unsigned int m, unsigned int n) {
    if (m <= n) return;

    unsigned int a = m * m - n * n;
    unsigned int b = 2 * m * n;
    unsigned int c = m * m + n * n;

    if (c > NMax) return;

    n_cache[c]++;

    // Generamos los hijos del nodo actual
    generate_PPT(2 * m - n, m);
    generate_PPT(2 * m + n, m);
    generate_PPT(m + 2 * n, n);
}

int main() {
    memset(n_cache, 0, sizeof(n_cache));

    // Llamada inicial con la primera terna pitagórica primitiva (3,4,5)
    generate_PPT(2, 1);

    unsigned int sum = 0;
    for (unsigned int i = 1; i <= NMax; i++) {
        sum += n_cache[i];
        n_cache[i] = sum;
    }

    unsigned int N;
    while (scanf("%u", &N), N) {
        printf("%u\n", n_cache[N]);
    }

    return 0;
}

