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
#include "functions.h"
#include "memory.h"

//? Tem que ser global?
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

  char **filepaths = NULL;

  if (args.file_given)
    filepaths = args.file_arg;

  // Analyses all the given files
  for (size_t i = 0; i < args.file_given; i++) check_file(filepaths[i]);

  // Analyses the files inside the given directory
  if (args.dir_given) {
    directorypath = args.dir_arg;
    check_dir(directorypath);
  }

  if (args.dir_given || args.batch_given) {
    int totalCount = okCount + mismatchCount + errorCount;
    printf("[SUMMARY] files analysed: %d; files OK: %d; mismatches: %d; errors: %d\n", totalCount, okCount, mismatchCount, errorCount);
  }

  return 0;
}