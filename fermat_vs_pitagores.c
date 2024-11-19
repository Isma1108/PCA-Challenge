// Versio inicial per veure si el jutge l'aprova

#include <stdio.h>

// Fast gcd using Euclidian algorithm, with recursion 
// O(log(min(a,b)))
int gcd(int a, int b) {return !b ? a : gcd(b, a%b);}

int gcd3(int a, int b, int c) {return gcd(gcd(a,b),c);}

int compute_triplets(int N) {
	int count = 0;
  for (int z = 1; z <= N; z++) {
  	for (int y = 1; y < z; y++) {
    	for (int x = 1; x < y; x++) {
      	if (x * x + y * y == z * z) {
        	if (gcd3(x, y, z) == 1) count++;
        }
      }
  	}
  }
  return count;
}

int main(int argc, char* argv[]) {
	int N;

	while (scanf("%d", &N), N) {
		int sol = compute_triplets(N);
		printf("%d\n", sol);
	}

	return 0;
}
