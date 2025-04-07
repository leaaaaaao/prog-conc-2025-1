#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEMCHECK(X) if ((X) == NULL) {fprintf(stderr, "ERRO: Erro de alocação de memória.\n"); exit(2);} 

int main (int argc, char **argv)
{
    int n;
    float *v1;
    float *v2;
    double produto_interno;

    if (argc != 3) {
        fprintf(stderr, "ERRO: Uso: %s <dimensao> <arquivo_de_saida>\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "ERRO: <dimensao> deve ser um inteiro positivo.\n");
        exit(1);
    }

    v1 = malloc(n * sizeof(*v1));
    v2 = malloc(n * sizeof(*v2));
    MEMCHECK(v1);
    MEMCHECK(v2);

    srand(time(NULL));
    produto_interno = 0;
    // gerando entradas aleatórias entre 0 e 1 e calculando o produto interno
    for (int i = 0; i < n; i++) {
        v1[i] = rand() / (float) RAND_MAX;
        v2[i] = rand() / (float) RAND_MAX;
        produto_interno += v1[i] * v2[i];
    }

    FILE *saida = fopen(argv[2], "wb");

    fwrite(&n, sizeof(n), 1, saida);
    fwrite(v1, sizeof(*v1), n, saida);
    fwrite(v2, sizeof(*v2), n, saida);
    fwrite(&produto_interno, sizeof(produto_interno), 1, saida);
    fclose(saida);

    return 0;
}

