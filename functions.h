/**
 * @file functions.h
 * @brief Functions to be used in main.c
 * @date 2021-11-01
 * @author Alexandre Leal Marques Batista Jerónimo - 2201799
 * @author Leonardo Jacinto Paulo - 2201783
 */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "validations.h"

void check_file(char *filepath);
void check_dir(char *directorypath);
void signal_handler(int signal);
void check_batch(char *filelist);

#endif /* FUNCTIONS_H_INCLUDED */
