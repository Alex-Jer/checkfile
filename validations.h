#ifndef VALIDATIONS_H_INCLUDED
#define VALIDATIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "args.h"
#include "debug.h"

#define MAX 124

int is_file_valid(char *filepath);
int is_file_supported(char *filename, char *extension, char mimetype[]);
void validate_extension(char *filename, char *extension, char *filetype);

#endif /* VALIDATIONS_H_INCLUDED */
