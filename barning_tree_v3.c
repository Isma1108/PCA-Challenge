#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define NMax 1000000

unsigned int n_cache[NMax + 1];

unsigned int stack[3*NMax + 1];
unsigned int* top = stack;


#define push(_x, _y, _z) {\
  *top = (unsigned int) (_x);\
  *(top+1) = (unsigned int) (_y);\
  *(top+2) = (unsigned int) (_z);\
  top += 3;\
}


#define pop() \
  (unsigned int) *(--top)


// Mejor version iterativa
void barning_PPT_tree_iterative() {
  unsigned int x, y, z, x2, y2, z2, z3, y2_z3, t1, t2, t3;
  while (top > stack) {
    // el numero (2 o 3) indica *2 i *3, aparecen z's *2 y *3
    z = pop();
    y = pop();
    x = pop();
    //printf("He hecho pop de: %u %u %u\n", x, y, z);

    n_cache[z]++;

    __builtin_prefetch(&n_cache[z], 0, 3);

    // Intento aprovechar operaciones
    x2 = x << 1;
    y2 = y << 1;
    z2 = z << 1;
    z3 = z2 + z;
    y2_z3 = y2 + z3;

    t1 = x - y2 + z2;
    t2 = x2 - y + z2;

    t3 = x2 - y2 + z3;
    if (t3 <= NMax) {
      //push(t1); push(t2); push(t3);
      push(t1,t2,t3)
      //printf("Pusheo tripleta: %u %u %u\n", t1, t2, t3);
    }

    t1 += (y2 << 1);

    t3 = x2 + y2_z3;
    if (t3 <= NMax) {
      //push(t1); push(t2+y2); push(t3);
      push(t1, t2+y2, t3)
      //printf("Pusheo tripleta: %u %u %u\n", t1, t2+y2, t3);
    }

    t3 = y2_z3 - x2;
    if (t3 <= NMax) {
      //push(t1-x2); push((z2 << 1)-t2); push(t3);
      push(t1-x2, (z2<<1)-t2, t3)
      //printf("Pusheo tripleta: %u %u %u\n", t1-x2,(z2<<1)-t2,t3);
    }
  }
}

int main(int argc, char* argv[]) {
    unsigned int N;
    memset(n_cache, 0, sizeof(n_cache));

    // Llamadas a funciones necesarias
    //push(3);
    //push(4);
    //push(5);
    push(3,4,5)
    barning_PPT_tree_iterative();

    // Procesamiento de n_cache
    for (unsigned int i = 5; i <= NMax; i += 4) {
        n_cache[i] += n_cache[i-1];
        n_cache[i+1] += n_cache[i];
        n_cache[i+2] += n_cache[i+1];
        n_cache[i+3] += n_cache[i+2];
    }

    char input_buffer[1024];
    char output_buffer[1024];

    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (sscanf(input_buffer, "%u", &N) != 1 || N == 0) {
            break;
        }
        sprintf(output_buffer, "%u\n", n_cache[N]);
        fputs(output_buffer, stdout);
    }

    return 0;
}

