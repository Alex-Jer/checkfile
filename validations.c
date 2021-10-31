#include "validations.h"

int okCount = 0;
int mismatchCount = 0;
int errorCount = 0;

// Check if the file is valid and ready to analyse
int is_file_valid(char *filepath) {
  // Check if the given file exists
  if (access(filepath, F_OK) != 0) {
    fprintf(stderr, "[ERROR] cannot open file '%s' -- No such file or directory\n", filepath);
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
int is_file_supported(char *filename, char *extension, char mimetype[]) {
  if (strcmp(extension, "pdf") && strcmp(extension, "gif") && strcmp(extension, "jpg") && strcmp(extension, "png") &&
      strcmp(extension, "mp4") && strcmp(extension, "zip") && strcmp(extension, "html")) {
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