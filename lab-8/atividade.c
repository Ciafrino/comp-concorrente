// Construido enquanto assistia as aulas assíncronas no youtube
// 
// Usei o meu código do lab 4 como base, visto que a ideia é essencialmente a mesma
// https://github.com/Ciafrino/comp-concorrente/blob/master/lab-4/atividade.c

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define NTHREADS 4

// Vamos mudar a ordem para poder controlar a execução das threads
// por meio dessa variável state

// state = 0 -> Seja bem-vindo
// state = 1 -> Fique a vontade
// state = 2 -> Sente-se por favor
// state = 3 -> Volte sempre
int state = 0;

// a ordem de execuçao pros estados vai ser (0 -> 1 -> 2 -> 3)
// ou (0 -> 2 -> 1 -> 3)

sem_t mutual_exc; // semaforo pra fazer exclusao
sem_t mid_state_restriction; // variavel de condição para os estados 1 e 2 iniciarem
sem_t last_state_restriction; // variavel de condiçao para o estado 3 iniciar

// Responsavel pela mensagem "Fique a vontade."
void *foo1() {
	sem_wait(&mid_state_restriction); // sync condicional usando semaforo

	printf("Fique a vontade.\n");

	sem_wait(&mutual_exc); 
	// seçao critica, podemos ir pro estado seguinte

	state += 1;

	sem_post(&mutual_exc);

	if (state == 3) {
		// se estamos no estado 3 significa que só resta o último estado a ser visitado (thread 3)
		sem_post(&last_state_restriction);
	} else if (state == 2) sem_post(&mid_state_restriction); // ainda tem a thread 4
	pthread_exit(NULL);
}

// Responsavel pela mensagem "Seja bem-vindo."
void *foo2() {
	printf("Seja bem-vindo!\n");

	sem_wait(&mutual_exc);
	// seçao critica
	
	state += 1;

	sem_post(&mutual_exc);

	// vamos ou pra thread 1 ou pra 4
	sem_post(&mid_state_restriction); 
	pthread_exit(NULL);
}

// Responsavel pela mensagem "Volte sempre!"
void *foo3() {
	sem_wait(&last_state_restriction); // sync condicional usando semaforo

	printf("Volte sempre!\n");

	sem_wait(&mutual_exc);
	// seçao critica

	state += 1;

	sem_post(&mutual_exc);
	pthread_exit(NULL);
}

// Responsavel pela mensagem "Sente-se por favor."
void *foo4() {
	sem_wait(&mid_state_restriction); // sync condicional usando semaforo

	printf("Sente-se por favor.\n");

	sem_wait(&mutual_exc);
	// seçao critica
	
	state += 1;

	sem_post(&mutual_exc);

	if (state == 3) {
		// se estamos no estado 3 significa que só resta o último estado a ser visitado (thread 3)
		sem_post(&last_state_restriction);
	} else if (state == 2) sem_post(&mid_state_restriction); // ainda tem a thread 2
	pthread_exit(NULL);
}

int main() {
	pthread_t tid[NTHREADS];

	// inicializaçao 
	sem_init(&mid_state_restriction, 0, 0);
	sem_init(&last_state_restriction, 0, 0);
	sem_init(&mutual_exc, 0, 1);

	// cria as threads
	pthread_create(tid, NULL, foo1, NULL);
	pthread_create(tid + 1, NULL, foo2, NULL);
	pthread_create(tid + 2, NULL, foo3, NULL);
	pthread_create(tid + 3, NULL, foo4, NULL);

	for (int id = 0; id < NTHREADS; ++id) {
		pthread_join(tid[id], NULL);
	}

	// remove as ferramentas
	sem_destroy(&mid_state_restriction);
	sem_destroy(&last_state_restriction);
	sem_destroy(&mutual_exc);

	return 0;
}
