/**
 * @file validations.h
 * @brief Multiple validation functions to ensure a healthy execution of the program
 * @date 2021-11-01
 * @author Alexandre Jerónimo
 * @author Leonardo Jacinto
 */

#ifndef VALIDATIONS_H_INCLUDED
#define VALIDATIONS_H_INCLUDED

#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"
#include "functions.h"
#include "memory.h"

#define MAX 124

/* Stores the date and time at which the program was initiated */
extern char timeFormatted[MAX];

/* Prepare the counters */
extern int okCount;
extern int misCount;
extern int errCount;

int is_file_valid(char *filepath);
int is_file_supported(char *filename, char *filetype, char mimetype[]);
int is_batch_valid(char *filelist, FILE *fp);
int is_dir_valid(char *directorypath, DIR *pdir);
void validate_extension(char *filename, char *extension, char *filetype);

#endif /* VALIDATIONS_H_INCLUDED */
