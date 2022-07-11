#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256
#define MAX_ARGS 5

int main() {
  char comando[MAX];
  char* args[1 + MAX_ARGS];
  int pid;

  while (1) {
    for (unsigned i = 0; i < MAX_ARGS + 1; i++)
      args[i] = '\0';

    printf("> ");
    fgets(comando, MAX, stdin);
    comando[strlen(comando) - 1] = '\0';

    char* nomeArquivo = NULL;
    unsigned op;
    char* str = strtok(comando, " ");
    unsigned i;
    for (i = 0; str != NULL && i < MAX_ARGS; str = strtok(NULL, " "), i ++){
        if(!strncmp(str, ">", 1)){
          nomeArquivo = strtok(NULL, " ");
          op = 0;
          break;
        } 
        if(!strncmp(str, "<", 1)){
          nomeArquivo = strtok(NULL, " ");
          op = 1;
          break;
        } 
        args[i] = str;
    }

    if (!strcmp(comando, "exit")) {
      exit(EXIT_SUCCESS);
    }

    pid = fork();
    if (pid) {
      waitpid(pid, NULL, 0);
    } else {
      if(nomeArquivo){
        FILE* arq;
        if(op == 0)
          arq = freopen(nomeArquivo, "w", stdout);
        else
          arq = freopen(nomeArquivo, "r", stdin);
          
        execvp(args[0], &args[0]);
        fclose(arq);
      }
      else
        execvp(args[0], &args[0]);
      printf("Erro ao executar comando!\n");
      exit(EXIT_FAILURE);
    }
  }
}
