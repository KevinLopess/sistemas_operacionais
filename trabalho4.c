/*
Faca um programa que imprima os nros primos existentes entre 0 e 99999.
O programa deve dividir o espaço de cálculo uniformemente entre as N threads da forma que vc achar mais efetivo do ponto de vista computacional.
e.x. 1bilhão de termos com 2 threads = 500milhões de termos em cada thread;
cada thread efetua uma soma parcial de forma autônoma;
para evitar o uso de mecanismos de sincronização, cada thread T[i] deve depositar seu resultado parcial na posição result[i] de um vetor de resultados parciais.
após o término das threads de cálculo, o programa principal soma os números primos encontrados
execute as soluções com N = {1, 2, 4, 8 e 16} threads
Marque o tempo necessário para calcular Pi para cada N e faça um gráﬁco de linhas (NxTempo) apresentado os resultados.
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 16
#define BILLION 1000000000.0

int result[NUM_THREADS] = {0};
long int soma = 0;


int primo (int t){
  int i,a,count;
  for (i=(99999/NUM_THREADS)*t;i<(99999/NUM_THREADS)*(t+1);i++)
  {
    count=0;
      for (a=1;a<=i;a++)
      {
       if (i%a==0)
         count++;
      }
    if (count==2)
      result[t] = result[t] + i;
  }
  pthread_exit(NULL);
}

int main () {
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    pthread_t threads[NUM_THREADS];

    for(int t=0; t < NUM_THREADS; t++){
        pthread_create(&threads[t], NULL, primo, t);
    }
    for (int t=0; t < NUM_THREADS; t++){
       pthread_join(threads[t], NULL);
     }
    for (int t=0; t < NUM_THREADS; t++){
      soma = soma + result[t];
      }

    clock_gettime(CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;

    printf("%d threads ----> %f s\n",NUM_THREADS,time_spent);
    printf("Soma total: %ld\n", soma);

    pthread_exit(NULL);
}
