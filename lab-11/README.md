# Documentação da classe FilaTarefas

A classe implementa o padrão produtor/consumidor, onde cada objeto da classe `MyPoolThreads` é uma thread consumidora. O número de threads consumidoras é definido por um parâmetro passado ao construtor da classe.

O método `execute` adiciona uma tarefa à última posição da lista `queue`, enquanto o método `run` das threads consumidoras executa a tarefa da primeira posição da lista. Ou seja, a ordem de execução das tarefas é "first in, first out".

A sincronização das threads é feita através da palavra-chave `synchronized` e por mecanismos de notificação. As threads consumidoras esperam enquanto a fila está vazia, e toda vez que uma tarefa é adicionada nela, é enviado um sinal (`queue.notify()`). 

O método `shutdown` notifica todas as threads consumidoras e liga a flag `this.shutdown`, que sinaliza que a execução dessas threads deve parar. Em seguida, o método realiza o join de todas essas threads.
