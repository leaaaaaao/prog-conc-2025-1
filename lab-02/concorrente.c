#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MEMCHECK(X) if ((X) == NULL) {fprintf(stderr, "ERRO: Erro de alocação de memória.\n"); exit(2);} 

typedef struct {
    float *v1;
    float *v2;
    int inicio;
    int final;
} args_t;

void *calcula_produto_interno(void *arg)
{
    args_t *argumentos = arg;
    float *v1 = argumentos->v1;
    float *v2 = argumentos->v2;
    
    double *retorno = malloc(sizeof(*retorno));
    MEMCHECK(retorno);
    for (int i = argumentos->inicio; i < argumentos->final; i++) {
        *retorno += v1[i] * v2[i];
    }
    
    pthread_exit(retorno);
}

int main (int argc, char **argv)
{
    int nthreads;
    pthread_t *threads;
    int dimensao;
    float *v1;
    float *v2;
    double produto_interno;
    double resposta;

    args_t *argumentos;

    // Valida parâmetros de linha de comando
    if (argc != 3) {
        fprintf(stderr, "ERRO: Uso: %s <n_threads> <arquivo_de_entrada>\n", argv[0]);
        exit(1);
    }

    nthreads = atoi(argv[1]);

    if (nthreads <= 0) {
        fprintf(stderr, "ERRO: <n_threads> deve ser um inteiro positivo.\n");
        exit(1);
    }

    // Aloca espaço para as threads e seus argumentos
    threads = malloc(sizeof(*threads) * nthreads);
    argumentos = malloc(sizeof(*argumentos) * nthreads);
    MEMCHECK(threads);
    MEMCHECK(argumentos);

    // Lê arquivo de entrada
    FILE *entrada = fopen(argv[2], "rb");
    fread(&dimensao, sizeof(dimensao), 1, entrada);

    v1 = malloc(dimensao * sizeof(*v1));
    v2 = malloc(dimensao * sizeof(*v2));
    MEMCHECK(v1);
    MEMCHECK(v2);

    fread(v1, sizeof(*v1), dimensao, entrada);
    fread(v2, sizeof(*v2), dimensao, entrada);
    fread(&resposta, sizeof(produto_interno), 1, entrada);

    fclose(entrada);

    /* Divide a entrada e inicia as threads. Se o tamanho da entrada
    * não for divisível por nthreads, deixa o resto para a última thread*/
    int fatia = dimensao / nthreads;
    for (int i = 0; i < nthreads; i++) {
        argumentos[i] = (args_t) { .v1 = v1, .v2 = v2, .inicio = (fatia * i), .final = (fatia * (i + 1)) };
        if (i == nthreads - 1) {
            argumentos[i].final = dimensao;
        }

        int falha = pthread_create(&threads[i], NULL, calcula_produto_interno, &argumentos[i]);
        if (falha) {
            fprintf(stderr, "ERRO: Erro ao criar a thread %d\n", i);
            exit(3);
        }
    }

    // Reúne o resultado das threads
    produto_interno = 0;
    for (int i = 0; i < nthreads; i++) {
        double *retorno;
        if (pthread_join(threads[i], (void **)&retorno)) {
            fprintf(stderr, "ERRO: Erro no join da thread %d\n", i);
            exit(4);
        }

        produto_interno += *retorno;
        free(retorno);
    }
    
    // Compara o resultado com o obtido no programa sequencial
    printf("Saída do programa concorrente: %lf\n", produto_interno);
    printf("Saída do programa sequencial: %lf\n", resposta);

    return 0;
}
