/**
 * @file main.c
 * @brief Description
 * @date 2021-1-1
 * @author name of author
 */

// https://stackoverflow.com/questions/7292642/grabbing-output-from-exec

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

#define MAX 124

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

  for (unsigned int i = 0; i < args.file_given; i++) {
    char mimetype[MAX] = "\0";
    char *filetype, *filename = filepath[i], *extension = strrchr(filepath[i], '.') + 1;

    // If the given file is inside a directory
    if (strchr(filepath[i], '/'))
      filename = strrchr(filepath[i], '/') + 1;

    // Check if the given file exists
    if (access(filepath[i], F_OK) != 0) {
      fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file or directory\n", filepath[i]);
      continue;
    }

    // If the given file's name doesn't have an extension
    if (!strchr(filepath[i], '.')) {
      fprintf(stderr, "[INFO] '%s': files with no extension are not supported by checkFile\n", filepath[i]);
      continue;
    }

    int link[2];
    pipe(link);
    pid_t pid = fork();

    switch (pid) {
      case -1: /* Error */
        ERROR(1, "fork() failed!\n");
        break;
      case 0: /* Child */
              // Change output to stdout
        if (dup2(link[1], STDOUT_FILENO) == -1)
          ERROR(1, "dup2() failed!\n");
        close(link[0]);
        close(link[1]);
        execlp("file", "file", "--mime-type", "-b", filepath[i], NULL);
        ERROR(1, "execlp() failed!\n");
        break;
      default: /* Parent */
        if (wait(NULL) == -1)
          ERROR(1, "wait() failed!\n");
        close(link[1]);

        // Read the output from the child (Interprocess Communication)
        if (read(link[0], mimetype, sizeof(mimetype)) == -1)
          ERROR(1, "read() failed!\n");

        mimetype[strlen(mimetype) - 1] = '\0';  // Removes the unecessary line break
        filetype = strchr(mimetype, '/') + 1;   // Gets the file type from the MIME type

        // Check if the file type is supported
        if (strcmp(extension, "pdf") && strcmp(extension, "gif") && strcmp(extension, "jpg") &&
            strcmp(extension, "png") && strcmp(extension, "mp4") && strcmp(extension, "zip") &&
            strcmp(extension, "html")) {
          fprintf(stderr, "[INFO] '%s': type '%s' is not supported by checkFile\n", filename, mimetype);
          continue;
        }

        // Check whether the extension matches the file type
        if (!strcmp(extension, filetype))
          printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, filetype);
        else if (strcmp(extension, filetype) && !strcmp(filetype, "jpeg") && !strcmp(extension, "jpg"))
          printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, filetype);
        else
          printf("[MISMATCH] '%s': extension is '%s', file type is '%s'\n", filename, extension, filetype);
    }
  }
  return 0;
}