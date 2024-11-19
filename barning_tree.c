#include <stdio.h>
#include <string.h>


char z_cache[1000001] __attribute__((aligned(64)));
unsigned int n_cache[1000001] __attribute__((aligned(64)));
#define NMax 1000000

// Después de llegar a un tiempo que no lograba mejorar usando los dos fors anidados con las variables m y n donde
// podemos obtener PPT's (a,b,c) con las formulas
// a = m^2 - n^2, b = 2mn, c = m^2 + n^2
// He optado por buscar alternativas mas rapidas en vez de seguir ofuscado con esa version, que ya usaba muchas
// optimizaciones, incluso SIMD.

// En Wikipedia, https://en.wikipedia.org/wiki/Tree_of_primitive_Pythagorean_triples, he visto que existe un árbol
// de tripletas pitagoricas primitivas, que se puede generar de la siguiente forma:
//
// Dada una PPT, podemos generar 3 mas multiplicandola por las siguientes matrices:
//
// A = [[1,-2,2], [2,-1,2], [2,-2,3]], B = [[1,2,2], [2,1,2], [2, 2,3]] y C = [[-1,2,2], [-2,1,2], [-2,2,3]]
//
// Se trata de algo demostrado por F. J. M. Barning. Entonces podemos evitar calculo de gcd's como en la version pasada,
// ir generando el árbol de forma recursiva e incrementar el valor de z's para las z que nos vamos encontrando.
// De esta forma podemos hacer una memoizacion de todo inicial, actualizando las sumas acumuladas después de la llamada a la funcion
// recursiva. Esto funciona si la tripleta inicial es una PPT, por lo que la llamada inicial es con la primera tripleta pitagorica
// que existe: (3,4,5). Para la cache de z's no tendremos un numero grande por z, asi que podemos usar chars.

void barning_PPT_tree(unsigned int x, unsigned int y, unsigned int z) {
  // Si z > NMax paramos la recursion
  if (z <= NMax) {
    z_cache[z]++;
    unsigned int xA, yA, zA, xB, yB, zB, xC, yC, zC;

    //Primer producto A x [x,y,z]
    xA = x - 2*y + 2*z;
    yA = 2*x - y + 2*z;
    zA = 2*x - 2*y + 3*z;
  
    //Segundo producto B x [x,y,z]
    xB = x + 2*y + 2*z;
    yB = 2*x + y + 2*z;
    zB = 2*x + 2*y + 3*z;
  
    //Tercer producto C x [x,y,z]
    xC = -x + 2*y + 2*z;
    yC = -2*x + y + 2*z;
    zC = -2*x + 2*y + 3*z;

    //Llamadas recursivas para seguir generando el árbol
    barning_PPT_tree(xA,yA,zA);
    barning_PPT_tree(xB,yB,zB);
    barning_PPT_tree(xC,yC,zC);
  }
}

int main(int argc, char* argv[]) {
  unsigned int N;

  memset(z_cache, 0, sizeof(z_cache));
  memset(n_cache, 0, sizeof(n_cache));

  barning_PPT_tree(3,4,5);

  for (unsigned int i = 5; i <= NMax; i++) n_cache[i] = z_cache[i] + n_cache[i-1];

	while (scanf("%u", &N), N) printf("%u\n", n_cache[N]);

	return 0;
}
