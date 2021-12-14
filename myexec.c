#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "parser/parse.h"
#define SIZE 255

int myexec(job *j, char **environ) {
  pid_t pid[SIZE];
  int fd[SIZE][2];
  int pipefd[SIZE][2];
  int i = 0;
  int cnt = 0;

  process *pr = j->process_list;

  for (i = 0; pr; pr = pr->next, i++) {
    
    cnt++;

    char **argv = pr->argument_list;
    char *input = pr->input_redirection;
    char *output = pr->output_redirection;
    write_option wop = pr->output_option;

    if (pr->next != NULL) {
      if (pipe(pipefd[i]) < 0) {
        perror("pipe");
        exit(1);
      }
    }
    
    if ((pid[i] = fork()) < 0) {
      perror("fork");
      exit(1);
    }

    if (pid[i] == 0) {

      if (!(i == 0 && pr->next == NULL)) {
        if (i == 0) {
          close(pipefd[i][0]);
          dup2(pipefd[i][1], 1);
          close(pipefd[i][1]);
        } else if (pr->next == NULL) {
          close(pipefd[i-1][1]);
          dup2(pipefd[i-1][0], 0);
          close(pipefd[i-1][0]);
        } else {
          close(pipefd[i-1][1]);
          close(pipefd[i][0]);
          dup2(pipefd[i-1][0], 0);
          dup2(pipefd[i][1], 1);
          close(pipefd[i-1][0]);
          close(pipefd[i][1]);
        }
      }

      if (input != NULL) {
        if ((fd[i][0] = open(input, O_RDONLY)) < 0) {
          perror("open_input");
          exit(1);
        }
        if (dup2(fd[i][0], 0) < 0) {
          perror("dup_input");
          exit(1);
        }
        close(fd[i][0]);
      }

      if (output != NULL) {
        int flag = (wop == TRUNC) ? O_TRUNC : O_APPEND;
        if ((fd[i][1] = open(output, O_CREAT | O_WRONLY | flag, 0644)) < 0) {
          perror("open_output");
          exit(1);
        }
        if (dup2(fd[i][1], 1) < 0) {
          perror("dup_output");
          exit(1);
        }
        close(fd[i][1]);
      }

      if (execve(argv[0], argv, environ) < 0) {
        perror("execve");
        exit(1);
      }
    }
    else {
      if (i != 0) {
        close(pipefd[i-1][0]);
        close(pipefd[i-1][1]);
      }
    }        
  }

  int status;

  for (i = 0; i < cnt; i++) {
    if (wait(&status) < 0) {
      perror("wait");
      exit(1);
    }
  }
  
  return 0;
}