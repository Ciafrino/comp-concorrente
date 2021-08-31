#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>

int nthreads = 0;

pthread_cond_t flag;
pthread_mutex_t mutex;

int blocked = 0; // # de threads bloqueadas
int iter = 0; // iteraçoes

int* sum_acc; //  vetor de soma acumulada, cada posiçao guarda o somatorio que a i-esima thread computou
int* values; // nosso array
int* foo; // variavel temporaria pra auxiliar

void barrier(int thread_id) {
	pthread_mutex_lock(&mutex);
	
	// quantidade de threads restantes para a barreira bloquear 
	int remaining = (nthreads - blocked);
	
	printf("\n-- thread %d chegou na barreira --\n", thread_id);

	// dois casos: não é a ultima thread a chegar na barreira
	// ou é a última
	if (remaining > 1) {
		printf("\n barreira bloqueou a thread %d\n", thread_id);
		blocked += 1;
		pthread_cond_wait(&flag, &mutex);
	} else {
		printf("\n barreira derrubada após a thread %d\n", thread_id);
		blocked = 0;
		pthread_cond_broadcast(&flag);
	}
	
	pthread_mutex_unlock(&mutex);
}

// calcula a soma do array todo
int get_acc_sum() {
	int S = 0;

	for (int i = 0; i < nthreads; ++i) {
		S += values[i];
	}

	return S;
}

// função executada pelas threads
void* go(void* args) {
	int thread_id = *(int *)args;

	int sum_acc = 0;

	for (int step = 0; step < nthreads; ++step) { 
		sum_acc += get_acc_sum(); // pega o somatorio

		barrier(thread_id); // sync

		values[thread_id] = rand() % 10; // atribui um numero aleatorio d [0, 10)

		barrier(thread_id); // sync
	}
	
	pthread_exit((void *)sum_acc);
}

int main() {
	srand(time(NULL)); // vamos usar para gerar os valores do array

	scanf("%d", &nthreads);
	
	// inicializa e aloca as estruturas
	pthread_t threads[nthreads];
	
	int* tid = (int *)malloc(sizeof(int) * nthreads);
	
	if (tid == NULL) {
		fprintf(stderr, "Erro de alocação\n");
		exit(1);
	}

	sum_acc = (int *)malloc(sizeof(int) * nthreads);
	
	if (sum_acc == NULL) {
		fprintf(stderr, "Erro de alocação\n");
		exit(1);
	}

	values = (int *)malloc(sizeof(int) * nthreads);

	if (values == NULL) {
		fprintf(stderr, "Erro de alocação\n");
		exit(1);
	}

	foo = (int *)malloc(sizeof(int));

	if (foo == NULL) {
		fprintf(stderr, "Erro de alocação\n");
		exit(1);
	}
	
	// inicializa o array
	for (int i = 0; i < nthreads; ++i) {
		values[i] = rand() % 10;
	}

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&flag, NULL);

	blocked = 0;
	iter = 0;

	// atribui o id que cada thread sera responsavel por
	for (int i = 0; i < nthreads; ++i) tid[i] = i;

	// cria as threads
	for (int i = 0; i < nthreads; ++i) {
		pthread_create(&threads[i], NULL, go, (void *)&tid[i]);
	}

	// espera as threads acabarem
	for (int i = 0; i < nthreads; ++i) {
		*foo = 0; // variavel temporaria que vai guardar o somatorio de cada thread

		pthread_join(threads[i], (void **)foo);

		sum_acc[i] = *foo; // coloco no array de soma acumulada

		iter += 1;
	}

	// checa se todos somatorio batem, se não bater vai retornar um erro de execução
	for (int i = 0; i + 1 < nthreads; ++i) {
		assert(sum_acc[i] == sum_acc[i + 1]);
	}

	printf("\033[0;31m");

	for (int i = 0; i < nthreads; ++i) {
		printf("thread %d: %d\n", i, sum_acc[i]);
	}

	printf("\033[0;36m");

	printf("\n--- Passou nos testes! ---\n");

	// desaloca
	free(values);
	free(foo);
	free(sum_acc);
	free(tid);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&flag);
	
	return 0;
}
