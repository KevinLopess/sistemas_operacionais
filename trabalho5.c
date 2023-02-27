#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define COMER 1
#define COZINHAR 2
sem_t *mutex, *sem_full, *sem_cozinhar;
int javalis_grelhados = 0;

void* process_k(void* unused){
    while(1){
        sem_wait(mutex);
        if (javalis_grelhados == 0) {
            printf("Javali k(0) acordou o cozinheiro\n");
            sem_post(sem_cozinhar);
            sem_wait(sem_full);
        }
        printf("Gaules k(0) comendo javali %d\n", javalis_grelhados);
        javalis_grelhados--;
        sleep(COMER);
        sem_post(mutex);
    }
    pthread_exit(NULL);
}

void* process_e(void* unused){
    while(1){
        sem_wait(mutex);
        if (javalis_grelhados == 0) {
            printf("Javali e(1) acordou o cozinheiro\n");
            sem_post(sem_cozinhar);
            sem_wait(sem_full);
        }
        printf("Gaules e(1) comendo javali %d\n", javalis_grelhados);
        javalis_grelhados--;
        sleep(COMER);
        sem_post(mutex);
        //sleep((rand()%4)+1);
    }
    pthread_exit(NULL);
}

void* process_v(void* unused){
    while(1){
        sem_wait(mutex);
        if (javalis_grelhados < 1) {
            printf("Javali v(2) acordou o cozinheiro\n");
            sem_post(sem_cozinhar);
            sem_wait(sem_full);
        }
        printf("Gaules v(2) comendo javali %d\n", javalis_grelhados);
        javalis_grelhados--;
        sleep(COMER);
        sem_post(mutex);
    }
    pthread_exit(NULL);
}

void* process_i(void* unused){
    while(1){
        sem_wait(mutex);
        if (javalis_grelhados < 1) {
            printf("Javali i(3) acordou o cozinheiro\n");
            sem_post(sem_cozinhar);
            sem_wait(sem_full);
        }
        printf("Gaules i(3) comendo javali %d\n", javalis_grelhados);
        javalis_grelhados--;
        sleep(COMER);
        sem_post(mutex);
    }
    pthread_exit(NULL);
}

void* process_n(void* unused){
    while(1){
        sem_wait(sem_cozinhar);
        while(javalis_grelhados < 10){
            javalis_grelhados++;
            printf("cozinheiro grelhando javali %d...\n", javalis_grelhados);
            sleep(COZINHAR);
        }
        sem_post(sem_full);
    }
    pthread_exit(NULL);
}

int main (void){

    sem_close(mutex);
    sem_close(sem_full);
    sem_close(sem_cozinhar);


    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    sem_full = sem_open("/sem_full", O_CREAT, 0644, 0);
    sem_cozinhar = sem_open("/sem_cozinhar", O_CREAT, 0644, 0);

    pthread_t thread_k, thread_e, thread_v, thread_i, thread_n;
    pthread_create(&thread_k,NULL,process_k,NULL);
    pthread_create(&thread_e,NULL,process_e,NULL);
    pthread_create(&thread_v,NULL,process_v,NULL);
    pthread_create(&thread_i,NULL,process_i,NULL);
    pthread_create(&thread_n,NULL,process_n,NULL);


    pthread_join(thread_k, NULL);
    pthread_join(thread_e, NULL);
    pthread_join(thread_v, NULL);
    pthread_join(thread_i, NULL);
    pthread_join(thread_n, NULL);


    sem_close(mutex);
    sem_close(sem_full);
    sem_close(sem_cozinhar);
}
