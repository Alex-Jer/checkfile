/**
 * @file main.c
 * @brief Description
 * @date 2021-1-1
 * @author name of author
 */

// https://stackoverflow.com/questions/7292642/grabbing-output-from-exec

#include <errno.h>
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

void handle_signal(int signal);

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

  struct sigaction act;
  char **filepaths = NULL;

  /* Regista rotina de tratamento de sinais  */
  act.sa_handler = handle_signal;

  /* Comportamento por omissao -- fidedigno */
  act.sa_flags = 0;

  /* Mascara sem sinais para nao os bloquear */
  sigemptyset(&act.sa_mask);

  /* Captura do sinal SIGUSR1 */
  if (sigaction(SIGUSR1, &act, NULL) < 0) {
    ERROR(1, "sigaction - SIGUSR1");
  }

  /* Captura do sinal SIGQUIT */
  if (sigaction(SIGQUIT, &act, NULL) < 0) {
    ERROR(2, "sigaction - SIGQUIT");
  }

  /* Informa utilizador do funcionamento do programa */
  printf("O programa esta pronto a receber os signals SIGQUIT e SIGUSR1\n");
  printf("PID do processo: %d\n", getpid());

  if (args.file_given)
    filepaths = args.file_arg;

  if (!args.batch_given)
    signal(SIGUSR1, SIG_IGN);

  // Analyses all the given files
  for (size_t i = 0; i < args.file_given; i++) check_file(filepaths[i]);

  // Analyses the files inside the given directory
  if (args.dir_given) {
    directorypath = args.dir_arg;
    check_dir(directorypath);
  }

  if (args.dir_given || args.batch_given) {
    int totalCount = okCount + mismatchCount;
    printf("[SUMMARY] files analysed: %d; files OK: %d; mismatches: %d; errors: %d\n", totalCount, okCount, mismatchCount, errorCount);
  }

  return 0;
}

void handle_signal(int signal) {
  int aux;
  /* Copia da variavel global errno */
  aux = errno;

  if (signal == SIGUSR1) {
    printf("Recebi o sinal SIGUSR1 (%d)\n", signal);
  } else if (signal == SIGQUIT) {
    printf("Captured SIGQUIT signal (sent by PID: %d). Use SIGINT to terminate application.\n", signal);
  }

  /* Restaura valor da variavel global errno */
  errno = aux;
}