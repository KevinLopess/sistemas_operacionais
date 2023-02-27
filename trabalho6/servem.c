#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>

#define NUM_SAVAGE 5
#define TIME_EAT 1

char name[5] = "KEVIN";

typedef struct shmbuf{
    int disp_comida;
    sem_t comer;
    sem_t cozinhar;
    sem_t pronto;
}shm_t;

shm_t *shared_mem;


void shared_memory(){
    int fd = shm_open("/sharedMem", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    ftruncate(fd, sizeof(shm_t));
    shared_mem = mmap(NULL, sizeof(shm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
}

void comer(long gaules){
    if(shared_mem->disp_comida > 0){
        shared_mem->disp_comida--;
        printf("%c comeu um javali, javali na mesa: %d\n", name[gaules],shared_mem->disp_comida);
    }
    else{
        sem_post(&shared_mem->cozinhar);
        printf("Sem javalis, selvagem %c acordando o cozinheiro.\n", name[gaules]);
        sem_wait(&shared_mem->pronto);
    }
}

void *selvagem(void *num_selvagem){
    long selvagem = (int)num_selvagem;
    while(1){
        sem_wait(&shared_mem->comer);
        comer(selvagem);
        sem_post(&shared_mem->comer);
        sleep(TIME_EAT);
    }
    pthread_exit(NULL);
}

int main(){

    shared_memory();

    pthread_t selvagens[NUM_SAVAGE];
    for(int i = 0; i < NUM_SAVAGE; i++)
        pthread_create(&selvagens[i], NULL, selvagem, (void*)i);


    for(int i = 0; i < NUM_SAVAGE; i++)
        pthread_join(selvagens[i], NULL);
    pthread_exit(NULL);
}
