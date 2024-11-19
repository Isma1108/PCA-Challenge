#include <stdio.h>
#include <string.h>

#define NMax 1000000
unsigned int n_cache[NMax + 1];

unsigned int stack[3000001];
unsigned int* top = stack;

#define push(x) *(top++) = (x)
#define pop() *(--top)

void generate_PPT_iterative() {
    // Inicializamos la pila con la primera terna pitagórica primitiva (3,4,5)
    push(2); // m
    push(1); // n

    while (top > stack) {
        unsigned int n = pop();
        unsigned int m = pop();

        unsigned int z = m * m + n * n;
        n_cache[z]++;

        // Calculamos los valores de los hijos y los agregamos a la pila
        unsigned int m2 = m << 1;
        unsigned int n2 = n << 1;
        unsigned int m2_n = m2 - n;
        unsigned int m2_n2 = m2 + n;
        unsigned int m_n2 = m + n2;

        if ((m2_n * m2_n + m * m) <= NMax) {
            push(m2_n); // m
            push(m);    // n
        }
        if ((m2_n2 * m2_n2 + m * m) <= NMax) {
            push(m2_n2); // m
            push(m);     // n
        }
        if ((m_n2 * m_n2 + n * n) <= NMax) {
            push(m_n2); // m
            push(n);    // n
        }
    }
}

int main() {
    memset(n_cache, 0, sizeof(n_cache));

    generate_PPT_iterative();

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

