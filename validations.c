/**
 * @file validations.c
 * @brief Multiple validation functions to ensure a healthy execution of the program
 * @date 2021-11-01
 * @author Alexandre Jerónimo
 * @author Leonardo Paulo
 */

#include "validations.h"

/* Initialise the counters */
int okCount = 0;
int misCount = 0;
int errCount = 0;

/**
 * @brief Return 1 if the file is valid, else return 0
 *
 * @param filepath char*
 * @return int
 */
int is_file_valid(char *filepath) {
  /* Check if the given file exists and if the user has enough permissions */
  if (access(filepath, F_OK) || access(filepath, R_OK)) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- %s\n", filepath, strerror(errno));
    errCount++;
    return 0;
  }

  /* If the given file's name doesn't have an extension */
  if (!strchr(filepath, '.')) {
    fprintf(stderr, "[INFO] '%s': files with no extension are not supported by checkFile\n", filepath);
    return 0;
  }

  return 1;
}

/**
 * @brief Return 1 if the file is supported, else return 0
 *
 * @param filename char*
 * @param filetype char*
 * @param mimetype char[]
 * @return int
 */
int is_file_supported(char *filename, char *filetype, char mimetype[]) {
  /* If the file type isn't PDF, GIF, JPG, PNG, MP4, ZIP or HTML */
  if (strcmp(filetype, "pdf") && strcmp(filetype, "gif") && strcmp(filetype, "jpeg") && strcmp(filetype, "png") &&
      strcmp(filetype, "mp4") && strcmp(filetype, "zip") && strcmp(filetype, "html")) {
    /* If the file is empty */
    if (!strcmp(filetype, "x-empty")) {
      fprintf(stderr, "[INFO] '%s': file must not be empty\n", filename);
      return 0;
    }
    fprintf(stderr, "[INFO] '%s': type '%s' is not supported by checkFile\n", filename, mimetype);
    return 0;
  }
  return 1;
}

/**
 * @brief Return 1 if the directory is valid, else return 0
 *
 * @param directorypath char*
 * @param pdir DIR*
 * @return int
 */
int is_dir_valid(char *directorypath, DIR *pdir) {
  if (!pdir) {
    fprintf(stderr, "[ERROR] cannot open dir '%s' -- %s\n", directorypath, strerror(errno));
    return 0;
  }
  return 1;
}

/**
 * @brief Return 1 if the file is valid, else return 0
 *
 * @param filelist char*
 * @param fp FILE*
 * @return int
 */
int is_batch_valid(char *filelist, FILE *fp) {
  struct stat statbuf;
  stat(filelist, &statbuf);
  if (!fp || S_ISDIR(statbuf.st_mode)) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- %s\n", filelist, strerror(errno));
    return 0;
  }
  return 1;
}

/**
 * @brief Check if the extension matches the file type
 *
 * @param filename char*
 * @param extension char*
 * @param filetype char*
 * @return void
 */
void validate_extension(char *filename, char *extension, char *filetype) {
  if (!strcmp(extension, filetype)) {
    printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, filetype);
    okCount++;
  } else if (strcmp(extension, filetype) && !strcmp(filetype, "jpeg") && !strcmp(extension, "jpg")) {
    printf("[OK] '%s': extension '%s' matches file type '%s'\n", filename, extension, filetype);
    okCount++;
  } else {
    printf("[MISMATCH] '%s': extension is '%s', file type is '%s'\n", filename, extension, filetype);
    misCount++;
  }
}