// Me baseei no código construido com a turma nas aulas assincronas da semana 2
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include"timer.h"

// Utilizarei a estratégia de uma thread por linha (alternado)

float *mat_left; // matriz da esquerda na multiplicaçao
float *mat_right; // matriz da direita
float *result; // resultado da multiplicaçao
int nthreads; // quantidade de threads

typedef struct {
   int id_thread; // id da thread
   int dim; // dimensao
} data_t;

void *tarefa(void *arg) {
    data_t* cur_thread = (data_t *)arg;
    
    // cada thread cuida de um grupo alternado de linhas
    const int N = cur_thread->dim;
    const int st = cur_thread->id_thread; 
    for (int k = 0; k < N; ++k) {
        for (int i = st; i < N; i += nthreads) {
            for (int j = 0; j < N; ++j) {
                result[i * N + k] += mat_left[i * N + j] * mat_right[j * N + k];
            }
        }
    }
	
    pthread_exit(NULL);
}

// testa o resultado da multiplicaçao de matrizes 
// se falhar, teremos um erro de execução
// no teste basta checar se todas as células da matriz
// result equivalem a "dim", isso porque estamos
// multiplicando duas matrizes com todas celulas igual a um.
void teste_mat(int dim) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            assert(result[i * dim + j] == dim); 
        }
    }
}

double st0, en0;
double st1, en1;
double st2, en2;

int main(int argc, char* argv[]) {
    pthread_t *tid; 
    data_t *arg; 
    int dim; // dimensao

    if (argc < 3) {
         printf("Digite: %s <dimensao da matriz> <quantidade de threads\n", argv[0]);
	 return 1;
    }

    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    nthreads = (nthreads > dim ? dim : nthreads);
     
    // inicialização
   
    GET_TIME(st0);

    mat_left = (float *)malloc(sizeof(float) * dim * dim);
    if (mat_left == NULL) {
        printf("--ERRO: malloc()\n");
	    return 2;
    }
    
    mat_right = (float *)malloc(sizeof(float) * dim * dim);
    if (mat_right == NULL) {
        printf("--ERRO: malloc()\n");
	    return 2;
    }

    result = (float *)malloc(sizeof(float) * dim * dim);
    if (result == NULL) {
        printf("--ERRO: malloc()\n");
	    return 2;
    }

    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            mat_left[i * dim + j] = 1;
            mat_right[i * dim + j] = 1;
            result[i] = 0;
        }
    }
   
    GET_TIME(en0);

    double timer = en0 - st0;

    printf("Tempo inicializaçao: %lf\n", timer);

    // criaçao de threads e algoritmo de multiplicaçao
    
    GET_TIME(st1);

    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        printf("ERRO: malloc\n");
        return 2;
    }

    arg = (data_t *)malloc(sizeof(data_t) * nthreads);
    if (arg == NULL) {
        printf("ERRO: malloc\n");
        return 2;
    }

    for (int id = 0; id < nthreads; ++id) {
        (arg + id)->id_thread = id;
        (arg + id)->dim = dim;
        if (pthread_create(tid + id, NULL, tarefa, (void *)(arg + id))) {
            printf("ERRO: pthread_create\n");
            return 3;
        }
    }

    for (int id = 0; id < nthreads; ++id) {
        pthread_join(*(tid + id), NULL);
    }

    GET_TIME(en1);

    double timer1 = en1 - st1;
 
    printf("Tempo execução: %lf\n", timer1);
    
    teste_mat(dim); 
    

    // finalizaçao
    GET_TIME(st2); 

    free(mat_left);
    free(mat_right);
    free(result);
    free(arg);
    free(tid);

    GET_TIME(en2);
    double timer2 = en2 - st2;
    printf("Tempo finalização: %lf\n", timer2);
}
