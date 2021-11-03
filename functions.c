/**
 * @file functions.c
 * @brief Functions to be used in main.c
 * @date 2021-11-01
 * @author Alexandre Jerónimo
 * @author Leonardo Paulo
 */

#include "functions.h"

char timeFormatted[MAX];
char *currentFile = NULL;
int fileCount = 0;

/**
 * @brief Handle SIGUSR1 and SIGQUIT
 *
 * @param signal int
 * @return void
 */
void signal_handler(int signal) {
  int aux = errno;
  if (signal == SIGUSR1) {
    printf("Program start time: %s\n", timeFormatted);
    printf("Processing file #%d: '%s'\n", fileCount, currentFile);
  } else if (signal == SIGQUIT)
    printf("Captured SIGQUIT signal (sent by PID: %d). Use SIGINT to terminate application.\n", signal);
  errno = aux;
}

/**
 * @brief Check if the extension of the file matches its MIME type
 *
 * @param filepath char*
 * @return void
 */
void check_file(char *filepath) {
  char mimetype[MAX] = "\0";
  char *filetype, *filename = filepath, *extension = strrchr(filepath, '.') + 1;

  /* If the given file is inside a directory */
  if (strchr(filepath, '/'))
    filename = strrchr(filepath, '/') + 1;

  /* If the given file is invalid */
  if (!is_file_valid(filepath))
    return;

  int link[2];
  pipe(link);
  pid_t pid = fork();

  switch (pid) {
    case -1: /* Error */
      ERROR(1, "fork() failed!\n");
      break;
    case 0: /* Child */
            /* Change output to stdout */
      if (dup2(link[1], STDOUT_FILENO) == -1)
        ERROR(1, "dup2() failed!\n");
      close(link[0]);
      close(link[1]);
      /* Run the file command */
      execlp("file", "file", "--mime-type", "-b", filepath, NULL);
      ERROR(1, "execlp() failed!\n");
      break;
    default: /* Parent */
      wait(NULL);
      /* Read the output from the child (IPC) */
      if (read(link[0], mimetype, sizeof(mimetype)) == -1)
        ERROR(1, "read() failed!\n");

      mimetype[strlen(mimetype) - 1] = '\0'; /* Remove the unecessary line break */
      filetype = strchr(mimetype, '/') + 1;  /* Get the file type from the MIME type */

      if (!is_file_supported(filename, filetype, mimetype))
        return;

      validate_extension(filename, extension, filetype);
      currentFile = filename;
      fileCount++;
  }
}

/**
 * @brief Validate the files of a directory
 *
 * @param directorypath char*
 * @return void
 */
void check_dir(char *directorypath) {
  DIR *pdir = opendir(directorypath);

  if (!is_dir_valid(directorypath, pdir))
    return;

  printf("[INFO] analyzing files of directory '%s'\n", directorypath);

  char filepath[MAX];
  struct dirent *pdirent;

  /* Process each entry */
  while ((pdirent = readdir(pdir))) {
    strcpy(filepath, directorypath);
    if (!strcmp(pdirent->d_name, "..") || !strcmp(pdirent->d_name, ".")) /* Ignore . and .. */
      continue;
    strcat(strcat(filepath, "/"), pdirent->d_name); /* Concatenate the filepath with the directory's name */
    check_file(filepath);
  }
  closedir(pdir);
}

/**
 * @brief Validate the files listed inside a text file
 *
 * @param filelist char*
 * @return void
 */
void check_batch(char *filelist) {
  FILE *fp = fopen(filelist, "r");
  size_t len = 0;
  ssize_t read;
  char *line = NULL;

  if (!is_batch_valid(filelist, fp))
    return;

  printf("[INFO] analyzing files listed in '%s'\n", filelist);

  /* Read the file line by line */
  while ((read = getline(&line, &len, fp)) != -1) {
    line[strcspn(line, "\n")] = 0; /* Trim the unecessary linebreak */
    check_file(line);
  }

  fclose(fp);
}