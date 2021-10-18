// https://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"
#include "memory.h"

#define MAX 100

char *filename = NULL;

int main(int argc, char *argv[]) {
  /* Gengetopt */
  struct gengetopt_args_info args;
  if (cmdline_parser(argc, argv, &args) != 0) exit(1);
  filename = args.file_arg;

  int fd;
  FILE *fptr = NULL;
  char cmd[MAX], mimeType[MAX];
  sprintf(cmd, "file --mime-type %s | cut -d' ' -f2", filename);

  // TODO: Concluir mensagem de erro
  if (argc < 2)
    ERROR(1, "[ERROR] must have at least one argument! usage: ...\n");

  /* Fork */
  pid_t pid;
  switch (pid = fork()) {
    case -1: /* Error */
      ERROR(1, "fork() failed\n");
      break;  //* Break desnecessário
    case 0:   /* Child */
      fd = open("tmp_output", O_WRONLY | O_CREAT, 0666);
      dup2(fd, 1);  // Opens the file descriptor on stdout (descriptor 1)
      close(fd);
      //! É considerado uma chamada à shell?
      execlp("bash", "bash", "-c", cmd, NULL);
      ERROR(1, "execlp() failed\n");
      break;      //* Break desnecessário
    default:      /* Parent */
      wait(NULL); /* Waits for the child process to end */
      fptr = fopen("tmp_output", "r");
      if (!fptr) ERROR(1, "Could not open tmp_file!\n");
      fgets(mimeType, 128, fptr);
      mimeType[strlen(mimeType) - 1] = '\0';
      //* if (strcmp(mimeType, "image/png") != 0) printf("Wrong file type!\n");
      //* printf("%s\n", mimeType);
      fclose(fptr);
      remove("tmp_output");
      break;
  }
  return 0;
}
