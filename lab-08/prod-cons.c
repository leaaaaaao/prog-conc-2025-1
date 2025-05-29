#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<math.h>
#include"prod-cons.h"

#define MEMCHECK(X) if((X) == NULL) {fprintf(stderr, "ERRO: Sem memória\n"); exit(1);}

long long int N;
long long int *buffer;
int num_slots;

sem_t slot_cheio, slot_vazio;
sem_t mutex_ninguem_lendo, mutex_processados;

void insere(int i) {
    static int in = 0;
    
    sem_wait(&slot_vazio);
    buffer[in] = i;
    in = (in + 1) % num_slots;
    sem_post(&slot_cheio);

    return;
}

void *produtor(void *)
{
    for (long long int i = 0; i < N; i++) {
        insere(i);
    }
    pthread_exit(NULL);
}

long long int retira(void) {
    static int out = 0;
    long long int retorno;

    sem_wait(&slot_cheio);
    sem_wait(&mutex_ninguem_lendo);

    retorno = buffer[out % num_slots];
    out++;

    sem_post(&mutex_ninguem_lendo);
    sem_post(&slot_vazio);

    return retorno;
}

void *consumidor(void *)
{
    static long long int processados = 0; // Para conferir se o trabalho já acabou
    long long int *primos_encontrados = malloc(sizeof(*primos_encontrados));

    MEMCHECK(primos_encontrados);
    *primos_encontrados = 0;

    while (1) {
        sem_wait(&mutex_processados);
        if (processados >= N) {
            sem_post(&mutex_processados);
            pthread_exit((void *)primos_encontrados);
        }
        processados++;
        sem_post(&mutex_processados);

        long long int primo = ehPrimo(retira());

        *primos_encontrados += primo;
    }
}

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
    long long int num_primos_melhor_consumidora = 0;
    int melhor_consumidora = -1;

    if (argc != 4) {
        fprintf(stderr, "ERRO: Uso: %s <N> <num_slots> <num_consumidores>\n", argv[0]);
        exit(2);
    }

    N = atoi(argv[1]);
    num_slots = atoi(argv[2]);
    int num_consumidores = atoi(argv[3]);

    buffer = malloc(sizeof(*buffer) * num_slots);
    MEMCHECK(buffer);

    sem_init(&slot_cheio, 0, 0);
    sem_init(&slot_vazio, 0, num_slots);

    // Mutexes
    sem_init(&mutex_ninguem_lendo, 0, 1);
    sem_init(&mutex_processados, 0, 1);

    pthread_t *tids = malloc(sizeof(pthread_t)* (1 + num_consumidores)); // 1 produtor

    // Inicializando threads
    for (int i = 0; i < num_consumidores; i++) {
        pthread_create(&tids[i], NULL, consumidor, NULL);
    }
    pthread_create(&tids[num_consumidores], NULL, produtor, NULL);

    // Joins
    for (int i = 0; i < num_consumidores; i++) {
        long long int primos_encontrados;
        void *valor_de_retorno;

        pthread_join(tids[i], &valor_de_retorno);
        
        primos_encontrados = *(long long int *)valor_de_retorno;
        free(valor_de_retorno);

        num_primos += primos_encontrados;
        if (primos_encontrados > num_primos_melhor_consumidora) {
            num_primos_melhor_consumidora = primos_encontrados;
            melhor_consumidora = i;
        }
    }
    pthread_join(tids[num_consumidores], NULL);

    // Limpando a memória
    sem_destroy(&slot_cheio);
    sem_destroy(&slot_vazio);

    sem_destroy(&mutex_ninguem_lendo);
    sem_destroy(&mutex_processados);

    free(buffer);
    free(tids);

    printf("Número de primos encontrados entre 0 e %ld: %ld\n", N, num_primos);
    printf("Thread que mais encontrou números primos: %ld (encontrou %ld primos)", melhor_consumidora, num_primos_melhor_consumidora);

    return 0;
}