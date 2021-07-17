// Me baseei no código construido com a turma na aula assíncrona 3
// para implementar a atividade 5 do lab-1
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>

// Vamos construir duas threads, uma par e outra ímpar
// A par vai iterar pelos elementos de índice par, da forma
// [2, 4, ..., N], já na thread ímpar vamos fazer [1, 3, ..., N-1]
// Para fazer isso podemos iterar com 2 * id + paridade, pra todo id
// em [0, N/2) com paridade sendo 1 se a thread for ímpar e 0 se for par
const short NTHREADS = 2;

typedef struct {
   int id_thread; // id da thread
   int cnt_thread; // quantidade de threads
   int parity; // 1 se for ímpar. 0 se for par
} data_t;

const int N = 10000; // quantidade de elementos no array

int values[100010];

// retorna o indice correspondente do array dependendo da paridade
// Se for a thread par, quero calcular para os indices pares (2 * indice)
// E análogo caso a thread seja ímpar (2 * indice + 1)
int get_index(int i, int parity) {
    return 2 * i + parity;
}

// eleva ao quadrado os elementos de índices pares ou ímpares do array values 
void *square(void *arg) {
    data_t* cur_thread = (data_t *)arg;
    
    printf("\nEstamos na thread %d!\n", cur_thread->parity);

    for (int i = 0; i < (N / 2); ++i) {
        int p = get_index(i, cur_thread->parity);
        values[p] *= values[p];
    }
   
    printf("\nSaimos da thread %d!\n", cur_thread->parity);

    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[2];
    data_t *arg;   

    // vamos inicializar o array values como (1, 2, ..., N) só para
    // executarmos um teste de sanidade
    for (int i = 0; i < N; ++i) {
        values[i] = i + 1;
    }
    
    for (int id = 0; id < NTHREADS; ++id) {
        arg = malloc(sizeof(data_t));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n");
            exit(-1);
        }

        arg->id_thread = id;
        arg->cnt_thread = NTHREADS; 
        arg->parity = (id & 1); // paridade da thread

        // cria a thread id
        if (pthread_create(&tid[id], NULL, square, (void*)arg)) {
            printf("--ERRO: pthread_create() %d\n", id);
            exit(-1);
        }
    }
   
    for (int id = 0; id < NTHREADS; ++id) {
        // espera a thread i acabar
        if (pthread_join(tid[id], NULL)) {
            printf("--ERRO: pthread_join() %d\n", id);
            exit(-1);
        }
    }
    
    // Erro de execução se o resultado não bater com o esperado
    for (int i = 0; i < N; ++i) {
        assert(values[i] == (i + 1) * (i + 1));
    }

    printf("\n\nPassou no teste!\n");
    pthread_exit(NULL);
}
