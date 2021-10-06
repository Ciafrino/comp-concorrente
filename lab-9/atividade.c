// Construido enquanto assistia as aulas assíncronas no youtube (Modulo 3 - semana 1)
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define N 10

sem_t slot_cheio;
sem_t slot_vazio;

sem_t mutex_prod; // mutual exc
sem_t mutex_cons;
	 
int cnt_cons;
int cnt_prod; 

int buffer[N];

void delay() { for(int a = -10000; a < 10000; ++a){} }

void insere(int item, int id) {
	static int in = 0;
	sem_wait(&slot_vazio);

	sem_wait(&mutex_prod);

	printf("Produtor %d: push(%d)\n", id, item);
	buffer[in] = item;
	in = (in + 1) % N;

	sem_post(&mutex_prod);
	sem_post(&slot_cheio);
}

int retira(int id) {
	int item;
	static int out = 0;

	sem_wait(&mutex_cons);
	
	for (int i = 0; i < N; ++i) {
		sem_wait(&slot_cheio);
		printf("Consumidor %d: pop(%d)\n", id, item);
		item = buffer[i];
		sem_post(&slot_vazio);
	}

	sem_post(&mutex_cons);
	return item;
}

void* consumidor(void* args) {
	int* id = (int *)args;

	printf("Consumidor %d aguardando\n", *id);
	
	while (1) { retira(*id); delay(); }
	
	pthread_exit(NULL);
}

void* produtor(void* args) {
	int* id = (int *)args;

	printf("Produtor %d aguardando\n", *id);
	while (1) { 
		int item = (rand() % 48);
		insere(item, *id); 
		delay();
	}

	pthread_exit(NULL);
}

int main() {
	srand(48);
	puts("Digite a quantidade de consumidores: ");
	scanf("%d", &cnt_cons);
	puts("Digite a quantidade de produtores: ");
	scanf("%d", &cnt_prod);

	pthread_t tid_cons[cnt_cons];
	pthread_t tid_prod[cnt_prod];

	sem_init(&slot_vazio, 0, N);
	sem_init(&slot_cheio, 0, 0);
	sem_init(&mutex_cons, 0, 1);
	sem_init(&mutex_prod, 0, 1);

	for (int i = 0; i < cnt_prod; ++i) {
		if (pthread_create(&tid_prod[i], NULL, produtor, (void *)&i)) {
			puts("ERRO- thread create");
			exit(-1);
		}
	}

	for (int i = 0; i < cnt_cons; ++i) {
		if (pthread_create(&tid_cons[i], NULL, consumidor, (void *)&i)) {
			puts("ERRO- thread create");
			exit(-1);
		}
	}

	for (int i = 0; i < cnt_prod; ++i) {
		if (pthread_join(tid_prod[i], NULL)) {
			puts("ERRO- pthread join");
			exit(-2);
		}
	}

	for (int i = 0; i < cnt_cons; ++i) {
		if (pthread_join(tid_cons[i], NULL)) {
			puts("ERRO- pthread join");
			exit(-2);
		}
	}

	sem_destroy(&slot_cheio);
	sem_destroy(&slot_vazio);
	sem_destroy(&mutex_cons);
	sem_destroy(&mutex_prod);
	return 0;
}


