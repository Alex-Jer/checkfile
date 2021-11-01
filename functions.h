/**
 * @file functions.h
 * @brief Functions to be used in main.c
 * @date 2021-11-01
 * @author Alexandre Jerónimo
 * @author Leonardo Paulo
 */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "validations.h"

#define MAX 124

void check_file(char *filepath);
void check_dir(char *directorypath);
void signal_handler(int signal);
void check_batch(char *filelist);

#endif /* FUNCTIONS_H_INCLUDED */
