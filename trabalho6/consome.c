#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>

#define QTD_COMIDA 10

typedef struct shmbuf{
    int disp_comida;
    sem_t comer;
    sem_t cozinhar;
    sem_t pronto;
}shm_t;

shm_t *shared_mem;

void semaforos(){
    sem_init(&shared_mem->cozinhar, 1, 0);
    sem_init(&shared_mem->comer, 1, 1);
    sem_init(&shared_mem->pronto, 1, 0);
    shared_mem->disp_comida = QTD_COMIDA;
}

void shared_memory(){
    int fd = shm_open("/sharedMem", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    ftruncate(fd, sizeof(shm_t));
    shared_mem = mmap(NULL, sizeof(shm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
}

void *Cozinheiro(){
    while(1){
        sem_wait(&shared_mem->cozinhar);
        if(shared_mem->disp_comida == 0){
            shared_mem->disp_comida = QTD_COMIDA;
            printf("Cozinheiro: %d javalis prontos\n", shared_mem->disp_comida);
            sleep(2);
        }
        else
            printf("Cozinheiro dormindo.\n");
        sem_post(&shared_mem->pronto);
    }
    pthread_exit(NULL);
}

int main(){
    shared_memory();
    semaforos();
    pthread_t cozinhar;

    pthread_create(&cozinhar, NULL, Cozinheiro, NULL);
    pthread_join(cozinhar, NULL);

    return 0;
}
