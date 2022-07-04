#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256

int main() {
  char comando[MAX];
  char* args[5];
  int pid;

  while (1) {
    printf("> ");
    fgets(comando, MAX, stdin);
    
    char *str = strtok(comando, " ");
    for (unsigned i = 0; str != NULL; str = strtok(NULL, " "), i ++){
      args[i] = str;
    }

    if (!strcmp(comando, "exit")) {
      exit(EXIT_SUCCESS);
    }

    pid = fork();
    if (pid) {
      waitpid(pid, NULL, 0);
    } else {
      execvp(comando, args);
      printf("Erro ao executar comando!\n");
      exit(EXIT_FAILURE);
    }
  }
}
