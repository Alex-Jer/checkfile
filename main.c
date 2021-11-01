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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"
#include "functions.h"
#include "memory.h"

int continua = 1;

int main(int argc, char *argv[]) {
  time_t rawtime;
  struct tm *tm;

  time(&rawtime);
  tm = localtime(&rawtime);
  strftime(timeFormatted, MAX, "%Y.%m.%d_%H:%M:%S", tm);

  /* Gengetopt */
  struct gengetopt_args_info args;
  if (cmdline_parser(argc, argv, &args) != 0)
    exit(1);

  if (argc < 2) {
    printf("Usage: checkFile [OPTION]...\nTry 'checkFile --help' for more information.\n");
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

  // Reads the given file and analyses its paths
  if (args.batch_given) {
    filelist = args.batch_arg;
    check_batch(filelist);
  } else
    // Ignores SIGUSR1
    signal(SIGUSR1, SIG_IGN);

  // Analyses the files inside the given directory
  if (args.dir_given) {
    char *directorypath = args.dir_arg;
    check_dir(directorypath);
  }

  // Analyses all the given files
  for (size_t i = 0; i < args.file_given; i++) check_file(filepaths[i]);

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
