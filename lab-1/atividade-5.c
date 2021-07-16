#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <assert.h>

const short NTHREADS = 2;

typedef struct {
   int id_thread; // id da thread
   int cnt_thread; // quantidade de threads
   int parity; // 1 se for ímpar. 0 se for par
} data_t;

const int N = 10000; 

int values[100010];

// retorna o indice correspondente do vetor dependendo da paridade
// Se for a thread par, quero calcular para os indices pares
// E análogo caso a thread seja ímpar
int get_index(int i, int parity) {
    return i << 1 | (parity & 1);
}

// eleva ao quadrado os elementos do array values de índices de certa paridade
void *square(void *arg) {
    data_t* cur_thread = (data_t *)arg;
    
    for (int i = 0; i < (N >> 1); ++i) {
        int p = get_index(i, cur_thread->parity);
        values[p] *= values[p];
    }

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
            printf("--ERRO: malloc\n");
            exit(-1);
        }

        arg->id_thread = id;
        arg->cnt_thread = NTHREADS; 
        arg->parity = id & 1; // paridade da thread

        if (pthread_create(&tid[id], NULL, square, (void*)arg)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
   
    for (int i = 0; i < NTHREADS; ++i) {
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthrad_join()\n");
            exit(-1);
        }
    }
    
    for (int i = 0; i < N; ++i) {
        assert(values[i] == (i + 1) * (i + 1));
        // printf("%d and %d\n", values[i], (i + 1) * (i + 1));
        // fflush(stdin);
    }

    printf("Passou no teste!\n");
    pthread_exit(NULL);
}
