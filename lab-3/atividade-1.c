// Me baseei no código construido com a turma nas aulas assincronas da semana 3
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include"timer.h"

typedef long long lint;

// Aqui vamos usar a ideia de dividir o array em blocos, 
// onde cada um desses blocos tem uma thread responsável por ele
// Para descobrirmos os ótimos globais (max e min) basta
// considerarmos os ótimos de cada bloco (local).

lint N; // quantidade de valores
lint nthreads; // quantidade de threads

float *values; // array valores

// margem de erro, por padrão 1e-3
const float eps = 1e-3;

// retorna -1 se a < b, 0 se a = b e 1 se a > b
int cmp_float(float a, float b) {
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

void *tarefa(void *arg) {
    lint cur_thread = (lint)arg;
    lint block = N / nthreads;
    lint st = cur_thread * block;
    lint en = st + block - 1;

    // 2 posições, primeira guarda minimo local e segunda maximo local
    float *minimax = (float *)malloc(sizeof(float) * 2);
   
    // vamos considerar -1e9 e 1e9 como bordas do intervalo, ou seja,
    // nao terão valores maiores ou iguais as bordas

    // inicializaçao
    *minimax = 1e9; 
    *(minimax + 1) = -1e9;

    while (st <= en) {
       if (cmp_float(*minimax, *(values + st)) == 1) *minimax = *(values + st);
       if (cmp_float(*(minimax + 1), *(values + st)) == -1) *(minimax + 1) = *(values + st);
       ++st;
   }

    //printf("%lld      %lld\n", *minimax, *(minimax + 1));
    pthread_exit((void *)minimax);
}


float *find_minimax() {
   static float minimax[2];
   minimax[0] = 1e9;
   minimax[1] = -1e9;
   
   // checagem linear por toda array procurando os valores otimos
   for (int i = 0; i < N; ++i) {
      if (cmp_float(minimax[0], *(values + i)) == 1) minimax[0] = *(values + i);
      if (cmp_float(minimax[1], *(values + i)) == -1) minimax[1] = *(values + i);
   }

   return minimax;
}

double st0, en0;
double st1, en1;
double st2, en2;

double seq_timer; // contador do algoritmo sequencial
double conc_timer; // contador do algoritmo concorrente

int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));
    pthread_t *tid; 

    if (argc < 3) {
         printf("Digite: %s <quantidade de numeros> <quantidade de threads\n", argv[0]);
	 return 1;
    }

    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);

     
    // inicialização
   
    GET_TIME(st0);

    values = (float *)malloc(sizeof(float) * N);
    if (values == NULL) {
        printf("--ERRO: malloc()\n");
	    return 2;
    }
    
    // https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
    for (int i = 0; i < N; ++i) {
       *(values + i) = (float)rand() / (float)(RAND_MAX / 1e4);
    }
   
    GET_TIME(en0);

    double timer = en0 - st0;

    printf("Tempo inicializaçao: %lf\n", timer);
   
    // criaçao de threads e inicio da contagem concorrente
    
    GET_TIME(st1);

    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        printf("ERRO: malloc\n");
        return 2;
    }

    for (int id = 0; id < nthreads; ++id) {
        if (pthread_create(tid + id, NULL, tarefa, (void *)id)) {
            printf("ERRO: pthread_create\n");
            return 3;
        }
    }


    // inicializa otimos globais
    float mi = 1e9;
    float ma = -1e9;

    // result vai guatdar os ótimos encontrados por cada uma das threads
    float *result;

    for (int id = 0; id < nthreads; ++id) {
        if (pthread_join(*(tid + id), (void **)&result)) {
            fprintf(stderr, "ERRO: pthread_join\n");
            return 4;
        }
        if (cmp_float(mi, *result) == 1) mi = *(result);
        if (cmp_float(ma, *(result + 1)) == -1) ma = *(result + 1);

        free(result);
    }

    GET_TIME(en1);

    conc_timer = en1 - st1;
 
    printf("Tempo concorrente: %lf\n", conc_timer);
    
    // inicio da execução do algoritmo sequencial

    GET_TIME(st2);
    float *seq;
    seq = find_minimax();
   
    GET_TIME(en2);
    seq_timer = en2 - st2;
    printf("Tempo sequencial: %lf\n", seq_timer);

    // checa se a resposta concorrente bate com a sequencial
    assert(cmp_float(*seq, mi) == 0);
    assert(cmp_float(*(seq + 1), ma) == 0);

    printf("Aceleração: %lf\n", seq_timer / conc_timer);

    // libera memoria
    free(values);
    free(tid);

    return 0;
}
