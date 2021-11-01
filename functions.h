#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <dirent.h>
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
#include "validations.h"

#define MAX 124

void check_file(char *filepath);
void check_dir(char *directorypath);
void signal_handler(int signal);
void check_batch(char *filelist);

#endif /* FUNCTIONS_H_INCLUDED */
