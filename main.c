/**
 * @file main.c
 * @brief Description
 * @date 2021-1-1
 * @author name of author
 */

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
char **filepath = NULL;
char *directorypath = NULL;

int main(int argc, char *argv[]) {
  /* Gengetopt */
  struct gengetopt_args_info args;
  if (cmdline_parser(argc, argv, &args) != 0)
    exit(1);

  if (argc < 2) {
    // TODO: Concluir mensagem de erro
    fprintf(stderr, "[ERROR] must have at least one argument! usage: ...\n");
    exit(1);
  }

  if (args.file_given)
    filepath = args.file_arg;
  if (args.dir_given)
    directorypath = args.dir_arg;

  int fd;
  FILE *fptr = NULL;
  char mimeType[MAX];

  int num_procs = args.file_given;
  for (int i = 0; i < num_procs; i++) {
    char *trueExtension, *extension = strrchr(filepath[i], '.') + 1, *filename = strrchr(filepath[i], '/') + 1;

    // Check if the given file exists
    if (access(filepath[i], F_OK) != 0) {
      fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file or directory\n", filepath[i]);
      exit(1);
    }

    pid_t pid = fork();
    switch (pid) {
      case -1: /* Error */
        ERROR(1, "fork() failed!\n");
        break;
      case 0: /* Child */
        //! validar
        fd = open("tmp_output", O_WRONLY | O_CREAT, 0666);
        // Open the file descriptor on stdout (descriptor 1)
        if (dup2(fd, 1) == -1)
          ERROR(1, "dup2() failed!\n");
        close(fd);
        execlp("file", "file", "--mime-type", "-b", filepath[i], NULL);
        ERROR(1, "execlp() failed!\n");
        break;
      default: /* Parent */
        if (wait(NULL) == -1)
          ERROR(1, "wait() failed!\n");
        if ((fptr = fopen("tmp_output", "r")) == NULL)
          ERROR(1, "fopen() failed!\n");
        fgets(mimeType, MAX, fptr);
        mimeType[strlen(mimeType) - 1] = '\0';
        if (strcmp(extension, "pdf") && strcmp(extension, "gif") && strcmp(extension, "jpg") &&
            strcmp(extension, "png") && strcmp(extension, "mp4") && strcmp(extension, "zip") &&
            strcmp(extension, "html")) {
          fprintf(stderr, "[INFO] '%s': type '%s' is not supported by checkFile\n", filename, mimeType);
          remove("tmp_output");
          fclose(fptr);
          continue;
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
  }
  return 0;
}