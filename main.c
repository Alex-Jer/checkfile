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

#define MAX 128

//? Tem que ser global?
char *filepath = NULL;

int main(int argc, char *argv[]) {
  /* Gengetopt */
  struct gengetopt_args_info args;
  if (cmdline_parser(argc, argv, &args) != 0)
    exit(1);
  filepath = args.file_arg;

  int fd;
  FILE *fptr = NULL;
  char mimeType[MAX];
  char *trueExtension, *extension = strrchr(filepath, '.') + 1, *filename = strrchr(filepath, '/') + 1;

  // TODO: Concluir mensagem de erro
  if (argc < 2) {
    fprintf(stderr, "[ERROR] must have at least one argument! usage: ...\n");
    exit(1);
  }

  // Check if the given file exists
  if (access(filepath, F_OK) != 0) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file or directory\n", filepath);
    exit(1);
  }

  pid_t pid;
  switch (pid = fork()) {
    case -1: /* Error */
      ERROR(1, "fork() failed!\n");
      break;  //* Break desnecessário?
    case 0:   /* Child */
      fd = open("tmp_output", O_WRONLY | O_CREAT, 0666);
      dup2(fd, 1);  // Opens the file descriptor on stdout (descriptor 1)
      close(fd);
      execl("/bin/file", "file", "--mime-type", "-b", filepath, NULL);
      ERROR(1, "execl() failed!\n");
      break;      //* Break desnecessário?
    default:      /* Parent */
      wait(NULL); /* Waits for the child process to end */
      fptr = fopen("tmp_output", "r");
      fgets(mimeType, MAX, fptr);
      mimeType[strlen(mimeType) - 1] = '\0';

      if (strcmp(extension, "pdf") && strcmp(extension, "gif") && strcmp(extension, "jpg") &&
          strcmp(extension, "png") && strcmp(extension, "mp4") && strcmp(extension, "zip") &&
          strcmp(extension, "html")) {
        fprintf(stderr, "[INFO] '%s': type '%s' is not supported by checkFile\n", filename, mimeType);
        remove("tmp_output");
        exit(1);
      }

      trueExtension = strchr(mimeType, '/') + 1;
      if (!strcmp(extension, trueExtension))
        printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, trueExtension);
      else if (strcmp(extension, trueExtension) && !strcmp(trueExtension, "jpeg") && !strcmp(extension, "jpg"))
        printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, trueExtension);
      else
        printf("[MISMATCH] '%s': extension is '%s', file type is '%s'\n", filename, extension, trueExtension);

      fclose(fptr);
      remove("tmp_output");
  }
  return 0;
}