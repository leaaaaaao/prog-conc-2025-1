#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int ehPrimo(long long int n) {
    long long int i;
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2) {
        if(n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main (int argc, char **argv) 
{
    long long int num_primos = 0;
    long long int N;
    if (argc != 2) {
        fprintf(stderr, "ERRO: Uso: %s <N>\n", argv[0]);
        exit(2);
    }

    N = atoi(argv[1]);

    for(int i = 0; i < N; i++) {
        num_primos += ehPrimo(i);
    }

    printf("Número de primos entre 0 e %ld: %ld", N, num_primos);
    return 0;
}