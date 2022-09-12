# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>

int d=1,i,filho2=0,mult;
pid_t childpid,childpid2;

void SIGTSTP_handler(){
 i = 1;
}

int main (void){
  childpid = fork();
  signal(SIGTSTP, SIGTSTP_handler);

  if (childpid == 0){
    printf("PID %d, PPID %d - F1\n",getpid(),getppid());
    printf("Filho 1: Executando 2*3\n");
    execl("mult","mult",NULL);
    exit(0);
    }
  else{
    childpid2 = fork();
    int status;
    waitpid(childpid,&status,0);
      if (childpid2 !=0 ){
        printf("PID %d, PPID %d - P\n",getpid(),getppid());
        printf("Pai: irei acordar filho 2.\n");
        mult = WEXITSTATUS(status);
        raise (SIGTSTP);
      }
  }

  while (i == 1){
    printf("PID %d, PPID %d - F2\n",getpid(),getppid());
    printf("Filho 2: Resultado: %d\n", mult);
    exit(0);
  }
}
