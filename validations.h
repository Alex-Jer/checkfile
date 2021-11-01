#ifndef VALIDATIONS_H_INCLUDED
#define VALIDATIONS_H_INCLUDED

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"

#define MAX 124

// Stores the date and time the program started
extern char timeFormatted[MAX];

// Initialise the counters
extern int okCount;
extern int mismatchCount;
extern int errorCount;

int is_file_valid(char *filepath);
int is_file_supported(char *filename, char *filetype, char mimetype[]);
void validate_extension(char *filename, char *extension, char *filetype);

#endif /* VALIDATIONS_H_INCLUDED */
