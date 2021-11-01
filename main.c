/**
 * @file main.c
 * @brief This program checks whether the file extension of the given file(s) matches its MIME type
 * @date 2021-11-01
 * @author Alexandre Jerónimo
 * @author Leonardo Paulo
 */

// https://stackoverflow.com/questions/7292642/grabbing-output-from-exec

#include "functions.h"

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

  /* Capture SIGUSR1 */
  if (sigaction(SIGUSR1, &act, NULL) < 0)
    ERROR(1, "sigaction - SIGUSR1");

  /* Capture SIGQUIT */
  if (sigaction(SIGQUIT, &act, NULL) < 0)
    ERROR(2, "sigaction - SIGQUIT");

  // printf("O programa esta pronto a receber os signals SIGQUIT e SIGUSR1\n");
  // printf("PID do processo: %d\n", getpid());

  if (args.file_given)
    filepaths = args.file_arg;

  /* Read the given file and analyses its paths */
  if (args.batch_given) {
    filelist = args.batch_arg;
    check_batch(filelist);
  } else
    /* Ignore SIGUSR1 */
    signal(SIGUSR1, SIG_IGN);

  /* Analyse the files inside the given directory */
  if (args.dir_given) {
    char *directorypath = args.dir_arg;
    check_dir(directorypath);
  }

  /* Analyse all the given files */
  for (size_t i = 0; i < args.file_given; i++) check_file(filepaths[i]);

  if (args.dir_given || args.batch_given) {
    int totalCount = okCount + misCount;
    printf("[SUMMARY] files analysed: %d; files OK: %d; mismatches: %d; errors: %d\n", totalCount, okCount, misCount, errCount);
  }

  // while (continua) {
  //   pause();
  //   printf("Pause interrompido\n");
  // }

  return 0;
}
