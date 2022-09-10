# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>

int d=1,i,filho2=0;
pid_t childpid,childpid2;

void SIGTSTP_handler(){
 filho2 = 1;
}


int main (void){
  childpid = fork();
  signal(SIGTSTP, SIGTSTP_handler);

  if (childpid == 0){
    printf("Filho 1 foi criado\n");
    d=100;
    for (i=0;i<=100;i++){
      printf("%d, PID %d, PPID %d - F1\n",d,getpid(),getppid());
      d++;
      sleep(1);
    }
    printf("Filho 1 vai morrer\n");
  }
  else{
  childpid2 = fork();
    if (childpid2 !=0 ){
      for (i=0;i<50;i++){
        printf("%d, PID %d, PPID %d - P\n",d,getpid(),getppid());
        d++;
        sleep(2);
      }
      printf("Processo pai vai morrer\n");
      raise (SIGTSTP);
    }
  }
  for (;;){
    if (filho2 == 1){
        printf("Filho 2 ativado, apos morte do pai\n");
        for (i=0;i<49;i++){
          printf("%d, PID %d, PPID %d - F2\n",d,getpid(),getppid());
          d++;
          sleep(2);
        }
        kill(getpid(),SIGKILL);
      }
  }
}




//filho 2 tem que ter wait
