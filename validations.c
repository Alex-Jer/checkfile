/**
 * @file validations.c
 * @brief Multiple validation functions to ensure a healthy execution of the program
 * @date 2021-11-01
 * @author Alexandre Jerónimo
 * @author Leonardo Paulo
 */

#include "validations.h"

int okCount = 0;
int mismatchCount = 0;
int errorCount = 0;

// Check if the file is valid and ready to analyse
int is_file_valid(char *filepath) {
  // Check if the given file exists
  if (access(filepath, F_OK)) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file or directory\n", filepath);
    errorCount++;
    return 0;
  }

  // Check if the user has enough permissions
  if (access(filepath, R_OK)) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- Permission denied\n", filepath);
    errorCount++;
    return 0;
  }

  // If the given file's name doesn't have an extension
  if (!strchr(filepath, '.')) {
    fprintf(stderr, "[INFO] '%s': files with no extension are not supported by checkFile\n", filepath);
    return 0;
  }
  return 1;
}

// Check if the file type is supported
int is_file_supported(char *filename, char *filetype, char mimetype[]) {
  if (strcmp(filetype, "pdf") && strcmp(filetype, "gif") && strcmp(filetype, "jpeg") && strcmp(filetype, "png") &&
      strcmp(filetype, "mp4") && strcmp(filetype, "zip") && strcmp(filetype, "html")) {
    fprintf(stderr, "[INFO] '%s': type '%s' is not supported by checkFile\n", filename, mimetype);
    return 0;
  }
  return 1;
}

// Check whether the extension matches the file type
void validate_extension(char *filename, char *extension, char *filetype) {
  if (!strcmp(extension, filetype)) {
    printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, filetype);
    okCount++;
  } else if (strcmp(extension, filetype) && !strcmp(filetype, "jpeg") && !strcmp(extension, "jpg")) {
    printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, filetype);
    okCount++;
  } else {
    printf("[MISMATCH] '%s': extension is '%s', file type is '%s'\n", filename, extension, filetype);
    mismatchCount++;
  }
}

// Checks if the directory exists
void validate_dir(char *directorypath, DIR *pdir) {
  if (!pdir) {
    fprintf(stderr, "[ERROR] cannot open dir '%s' -- No such file or directory\n", directorypath);
    exit(1);
  }
}

// Checks if the file exists and if it's not a directory
void validate_batch(char *filelist, FILE *fp, struct stat statbuf) {
  if (!fp || S_ISDIR(statbuf.st_mode)) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file or directory\n", filelist);
    exit(1);
  }
}
