# Lab 02

## Descrição
Entrega do lab 3, que consiste em:

1. Um programa sequencial em C que gera aleatoriamente dois vetores de floats de dimensão _N_. Esse programa também escreve o valor de _N_ (tipo inteiro), os dois vetores (vetores de float) e o produto interno desses vetores em um arquivo binário.

1. Um programa concorrente que recebe como entrada o número de threads e um nome de arquivo. Esse programa carrega do arquivo de entrada a dimensão _N_, os dois vetores _N_-dimensionais, e o produto interno calculado de forma sequencial; calcula o produto interno dividindo a tarefa entre as threads de forma balanceada e ao final, compara o resultado obtido com o calculado previamente pelo programa sequencial.

## Uso
### sequencial.c
Recebe como parâmetros de linha de comando a dimensão dos vetores e o nome do arquivo de saída, no seguinte formato:
```
gcc -o sequencial sequencial.c -Wall -Wextra -std=c99
./sequencial <dimensao> <arquivo_de_saida>
```

Gera um arquivo binário no formato especificado e com o nome passado no parâmetro `<arquivo_de_saida>`. OBS: `<dimensao>` deve ser um inteiro maior ou igual a 1.

### concorrente.c
Recebe como parâmetros de linha de comando o número de threads e o nome do arquivo de entrada, que deve ter o formato especificado anteriormente.
```
gcc -o concorrente concorrente.c -Wall -Wextra -std=c99
./concorrente <n_threads> <arquivo_de_entrada>
```

Imprime o resultado obtido pelo programa sequencial (lido do arquivo), o resultado calculado de forma concorrente e a variação relativa.

## Resultado observado


