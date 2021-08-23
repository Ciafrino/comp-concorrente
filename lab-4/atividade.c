// Construido enquanto assistia as aulas assíncronas no youtube
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

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

pthread_mutex_t cur_mutex;
pthread_cond_t mid_state_restriction; // variavel de condição para os estados 1 e 2 iniciarem
pthread_cond_t last_state_restriction; // variavel de condiçao para o estado 3 iniciar

// Responsavel pela mensagem "Fique a vontade."
void *foo1() {
	pthread_mutex_lock(&cur_mutex);

	// única restrição é que essa thread seja executada antes da t3 e depois da t1, logo,
	
	while (state != 1 && state != 2) pthread_cond_wait(&mid_state_restriction, &cur_mutex);

	printf("Fique a vontade.\n");

	state += 1;

	// libera o estado final se for a vez dele (2 e 3 ja tiverem sido executados)
	if (state == 3) pthread_cond_signal(&last_state_restriction);

	pthread_mutex_unlock(&cur_mutex);
	pthread_exit(NULL);
}

// Responsavel pela mensagem "Seja bem-vindo."
void *foo2() {
	pthread_mutex_lock(&cur_mutex);

	printf("Seja bem-vindo!\n");
	state += 1;

	pthread_mutex_unlock(&cur_mutex);

	// agora mid_state (state 1 ou 2) tá liberado 
	pthread_cond_broadcast(&mid_state_restriction);
	
	pthread_exit(NULL);
}

// Responsavel pela mensagem "Volte sempre!"
void *foo3() {
	pthread_mutex_lock(&cur_mutex);

	// essa thread tem que ser a última, então ela só pode ser executada quando o state for 3
	while (state != 3) pthread_cond_wait(&last_state_restriction, &cur_mutex);

	printf("Volte sempre!\n");

	state += 1;

	pthread_mutex_unlock(&cur_mutex);
	pthread_exit(NULL);
}

// Responsavel pela mensagem "Sente-se por favor."
void *foo4() {
	pthread_mutex_lock(&cur_mutex);

	// exatamente como a thread 1, tanto faz a ordem entre elas..
	
	while (state != 1 && state != 2) pthread_cond_wait(&mid_state_restriction, &cur_mutex);

	printf("Sente-se por favor.\n");

	state += 1;

	// libera o estado final se for a vez dele (2 e 3 ja tiverem sido executados)
	if (state == 3) pthread_cond_signal(&last_state_restriction);

	pthread_mutex_unlock(&cur_mutex);
	pthread_exit(NULL);
}

int main() {
	pthread_t tid[NTHREADS];

	// inicializa as ferramentas 
	pthread_mutex_init(&cur_mutex, NULL);
	pthread_cond_init(&mid_state_restriction, NULL);
	pthread_cond_init(&last_state_restriction, NULL);

	// cria as threads
	pthread_create(tid, NULL, foo1, NULL);
	pthread_create(tid + 1, NULL, foo2, NULL);
	pthread_create(tid + 2, NULL, foo3, NULL);
	pthread_create(tid + 3, NULL, foo4, NULL);

	for (int id = 0; id < NTHREADS; ++id) {
		pthread_join(tid[id], NULL);
	}

	// remove as ferramentas
	pthread_mutex_destroy(&cur_mutex);
	pthread_cond_destroy(&mid_state_restriction);
	pthread_cond_destroy(&last_state_restriction);
	return 0;
}
