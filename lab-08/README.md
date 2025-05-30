# Lab 08

## Descrição
Entrega da atividade 6 lab 8, que consiste em um programa concorrente que segue o paradigma produtor/consumidor. Apenas uma thread produtora gera N números inteiros (de 0 a N-1), e os deposita - um de cada vez - em um canal com capacidade para M inteiros. Há C threads consumidoras que retiram os números - também um de cada vez - desse canal e avaliam sua primalidade. A saída do programa é o número total de primos nesse intervalo e o índice da thread consumidora "vencedora", que encontrou mais números primos.

Adicionalmente, incluí um programa sequencial que computa a primalidade de todos os números de 0 a N - 1 e imprime o número de primos nesse intervalo, de forma a avaliar a corretude do programa concorrente por comparação com o sequencial, que é bem mais simples.

## Uso
### primos-seq.c
Recebe como parâmetros de linha de comando o valor de N, definido acima:
```
gcc -o sequencial primos-seq.c -lm
./sequencial <N>
```

Tem como saída apenas o número de primos entre 0 e N - 1.

### prod-cons.c
Recebe como parâmetros de linha de comando o valor de N, o tamanho do buffer (M) e o número de threads consumidoras (C).
```
gcc -o prod-cons prod-cons.c -lm -lpthread
./prod-cons <N> <num_slots> <num_consumidores>
```

Imprime tanto o número de primos entre 0 e N - 1 quanto o índice da thread vencedora. Também inclui quantos primos essa thread encontrou.

## Resultado observado
Os valores obtidos para diferentes valores de N, M e C no programa concorrente foram idênticos aos obtidos para os respectivos valores de N no programa sequencial, indicando que o programa está correto.