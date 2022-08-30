# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int cont=0, alrm = 0;


void SIGINT_handler(){
  printf("SIGINT sendo ignorado...\n");
}

void SIGTSTP_handler(){
  cont = cont+1;
  printf("Contagem de sinais SIGTSTP: %d\n",cont);

  if (cont == 12){
    printf("12 sinais de SIGTSTP.\n");
    kill(getpid(),SIGKILL);
  }
}

void SIGPIPE_handler(){
  printf("Contador SIGTSTP zerado.\n");
  cont = 0;
}

void SIGALRM_handler(){
  raise (SIGTSTP);
  //gerar sinal SIGTSTP
  printf("Enviando sinal SIGTSTP.\n");
  alrm = 0;
}


int main(void){

  signal(SIGINT, SIGINT_handler);
  signal(SIGTSTP, SIGTSTP_handler);
  signal(SIGPIPE, SIGPIPE_handler);
  signal(SIGALRM,SIGALRM_handler);
  printf("PID do processo: %d\n", getpid());

  for (;;){
    if (alrm == 0){
      alrm= 1;
      alarm(10);
    }
  }
}
