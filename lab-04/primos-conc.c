#include<stdio.h>
#include<stdlib.h> 
#include<pthread.h>
#include<math.h>

#define MEMCHECK(X) if ((X) == NULL) { fprintf(stderr, "ERRO: Sem memória\n"); exit(1); }

int ehPrimo(long long int);

long long int num_testados = 0;
long long int num_primos = 0;
long long int N;

pthread_mutex_t mutex_num_testados;
pthread_mutex_t mutex_num_primos;

void *loop_verifica_primalidade(void *) {
    long long int n;

    pthread_mutex_lock(&mutex_num_testados);
        n = num_testados;
        num_testados++;
    pthread_mutex_unlock(&mutex_num_testados);

    while (n <= N) {
        if (ehPrimo(n)) {
            pthread_mutex_lock(&mutex_num_primos);
                num_primos++;
            pthread_mutex_unlock(&mutex_num_primos);
        }

        pthread_mutex_lock(&mutex_num_testados);
            n = num_testados;
            num_testados++;
        pthread_mutex_unlock(&mutex_num_testados);
    }

    return NULL;
}

int ehPrimo(long long int n) {
    int i;

    if (n <= 1) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    for (i = 3; i < sqrt(n) + 1; i += 2) {
        if(n % i== 0) {
            return 0;
        } 
    }
    return 1;
}

int main (int argc, char **argv) {
    pthread_t *tid; // Identificadores das threads no sistema
    int nthreads; // Quantidade de threads (passada linha de comando)

    if(argc != 3) {
        fprintf(stderr, "Uso: %s <numero de threads> <N>\n", argv[0]);
        return 1;
    }

    pthread_mutex_init(&mutex_num_testados, NULL);
    pthread_mutex_init(&mutex_num_primos, NULL);

    nthreads = atoi(argv[1]);
    N = atoll(argv[2]);


    tid = malloc(sizeof(*tid) * nthreads);
    MEMCHECK(tid);

    for (int i = 0; i < nthreads; i++) {
        int erro = pthread_create(&tid[i], NULL, loop_verifica_primalidade, NULL);

        if (erro) {
            fprintf(stderr, "ERRO: Erro ao criar uma thread\n");
            exit(2);
        }
    }

    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "ERRO: Erro ao unir uma thread\n"); 
            exit(3); 
        } 
    }

    printf("Há %lld primos entre 0 e %lld\n", num_primos, N);

    return 0;
}
