#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"
#include "memory.h"

char *filename = NULL;

int main(int argc, char *argv[]) {
  /* Gengetopt */
  struct gengetopt_args_info args;
  if (cmdline_parser(argc, argv, &args) != 0) exit(1);
  filename = args.file_arg;

  //* Array fixo?
  char command[100];
  sprintf(command, "file --mime-type %s | cut -d' ' -f2", filename);

  // TODO: Concluir mensagem de erro
  if (argc < 2) ERROR(1, "[ERROR] must have at least one argument! usage: ...\n");

  /* Fork */
  pid_t pid;
  switch (pid = fork()) {
    case -1: /* Error */
      ERROR(1, "fork() failed\n");
      break;
    case 0: /* Child */
      execlp("bash", "bash", "-c", command, NULL);
      //? Como capturar o stdout da função execlp?
      ERROR(1, "execlp() failed\n");
      break;
    default: /* Parent */
      wait(NULL);
      break;
  }
  return 0;
}
