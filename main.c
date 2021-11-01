/**
 * @file main.c
 * @brief Description
 * @date 2021-1-1
 * @author name of author
 */

// https://stackoverflow.com/questions/7292642/grabbing-output-from-exec

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"
#include "functions.h"
#include "memory.h"

//? Tem que ser global?
char *directorypath = NULL;

// int continua = 1;

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

  char **filepaths = NULL;
  char *filelist = NULL;
  struct sigaction act;

  act.sa_handler = signal_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  // Captures SIGUSR1
  if (sigaction(SIGUSR1, &act, NULL) < 0)
    ERROR(1, "sigaction - SIGUSR1");

  // Captures SIGQUIT
  if (sigaction(SIGQUIT, &act, NULL) < 0)
    ERROR(2, "sigaction - SIGQUIT");

  // printf("O programa esta pronto a receber os signals SIGQUIT e SIGUSR1\n");
  // printf("PID do processo: %d\n", getpid());

  if (args.file_given)
    filepaths = args.file_arg;

  if (args.batch_given)
    filelist = args.batch_arg;
  else
    signal(SIGUSR1, SIG_IGN);

  // Analyses all the given files
  for (size_t i = 0; i < args.file_given; i++) check_file(filepaths[i]);

  // Analyses the files inside the given directory
  if (args.dir_given) {
    directorypath = args.dir_arg;
    check_dir(directorypath);
  }

  if (args.batch_given) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    struct stat statbuf;

    stat(filelist, &statbuf);
    fp = fopen(filelist, "r");

    // Checks if the file exists and if it's not a directory
    if (!fp || S_ISDIR(statbuf.st_mode)) {
      fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file\n", filelist);
      exit(1);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
      line[strcspn(line, "\n")] = 0;
      check_file(line);
    }

    fclose(fp);
  }

  if (args.dir_given || args.batch_given) {
    int totalCount = okCount + mismatchCount;
    printf("[SUMMARY] files analysed: %d; files OK: %d; mismatches: %d; errors: %d\n", totalCount, okCount, mismatchCount, errorCount);
  }

  // while (continua) {
  //   pause();
  //   printf("Pause interrompido\n");
  // }

  return 0;
}
