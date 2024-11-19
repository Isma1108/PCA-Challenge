#include <stdio.h>
#include <string.h>

#define NMax 1000000

unsigned int n_cache[NMax + 1];

unsigned int stack[3*NMax + 1];
unsigned int* top = stack;

#define push(x) \
  *(top++) = (unsigned int) (x)

#define pop() \
  (unsigned int) *(--top)


// Mejor version iterativa
void barning_PPT_tree_iterative() {
  while (top > stack) {
    // el numero (2 o 3) indica *2 i *3, aparecen z's *2 y *3
    unsigned int x, y, z, x2, y2, z2, z3, y2_z3, t1, t2, t3;
    z = pop();
    y = pop();
    x = pop();
    //printf("He hecho pop de: %u %u %u\n", x, y, z);

    n_cache[z]++;

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
      push(t1); push(t2); push(t3);
      //printf("Pusheo tripleta: %u %u %u\n", t1, t2, t3);
    }

    t1 += (y2 << 1);

    t3 = x2 + y2_z3;
    if (t3 <= NMax) {
      push(t1); push(t2+y2); push(t3);
      //printf("Pusheo tripleta: %u %u %u\n", t1, t2+y2, t3);
    }

    t3 = y2_z3 - x2;
    if (t3 <= NMax) {
      push(t1-x2); push((z2 << 1)-t2); push(t3);
      //printf("Pusheo tripleta: %u %u %u\n", t1-x2,(z2<<1)-t2,t3);
    }
  }
}



int main(int argc, char* argv[]) {

  unsigned int N;
  memset(n_cache, 0, sizeof(n_cache));

  //barning_PPT_tree(3,4,5);
  push(3);
  push(4);
  push(5);
  barning_PPT_tree_iterative();

  for (unsigned int i = 5; i <= NMax; i++) n_cache[i] += n_cache[i-1];

	while (scanf("%u", &N), N) printf("%u\n", n_cache[N]);

	return 0;
}
