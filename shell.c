#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256
#define MAX_ARGS 5
typedef enum {false, true} bool;

int main() {
  char comando[MAX];
  char* args[MAX_ARGS + 1];
  int pid;

  while (1) {
    for (unsigned j = 0; j < MAX_ARGS + 1; j++)
      args[j] = '\0';

    printf("> ");
    fgets(comando, MAX, stdin);
    comando[strlen(comando) - 1] = '\0';

    char* str = strtok(comando, " "), *nomeArquivo = NULL;
    unsigned i, op;
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

    bool segundoPlano = false;
    char* ultimoCaractere = args[i - 1] + strlen(args[i - 1]) - 1;
    if(!strcmp(ultimoCaractere, "&")){
      *ultimoCaractere = '\0';  
      if(!strcmp(args[i - 1], ""))
        args[i - 1] = NULL;
      segundoPlano = true;
    }
    
    pid = fork();
    if (pid) {
      if(!segundoPlano)
        waitpid(pid, NULL, 0);
    } else {
      //se alguma operação de arquivo for selecionada
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
