#include "functions.h"

// Checks whether the extension of file given matches its MIME type
void check_file(char *filepath) {
  char mimetype[MAX] = "\0";
  char *filetype, *filename = filepath, *extension = strrchr(filepath, '.') + 1;

  // If the given file is inside a directory
  if (strchr(filepath, '/'))
    filename = strrchr(filepath, '/') + 1;

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
            // Change output to stdout
      if (dup2(link[1], STDOUT_FILENO) == -1)
        ERROR(1, "dup2() failed!\n");
      close(link[0]);
      close(link[1]);
      execlp("file", "file", "--mime-type", "-b", filepath, NULL);
      ERROR(1, "execlp() failed!\n");
      break;
    default: /* Parent */
      if (wait(NULL) == -1)
        ERROR(1, "wait() failed!\n");
      close(link[1]);

      // Read the output from the child (Interprocess Communication)
      if (read(link[0], mimetype, sizeof(mimetype)) == -1)
        ERROR(1, "read() failed!\n");

      mimetype[strlen(mimetype) - 1] = '\0';  // Removes the unecessary line break
      filetype = strchr(mimetype, '/') + 1;   // Gets the file type from the MIME type

      if (!is_file_supported(filename, filetype, mimetype))
        return;

      validate_extension(filename, extension, filetype);
  }
}

// Validates the files of a directory
void check_dir(char *directorypath) {
  // Ensure we can open the directory
  DIR *pdir;
  pdir = opendir(directorypath);

  if (!pdir) {
    fprintf(stderr, "[ERROR] cannot open dir '%s' -- No such file or directory\n", directorypath);
    exit(1);
  }

  char filepath[MAX];
  struct dirent *pdirent;

  // Process each entry
  while ((pdirent = readdir(pdir))) {
    strcpy(filepath, directorypath);
    if (!strcmp(pdirent->d_name, "..") || !strcmp(pdirent->d_name, "."))
      continue;
    strcat(strcat(filepath, "/"), pdirent->d_name);
    check_file(filepath);
  }

  // Close directory
  closedir(pdir);
}

void signal_handler(int signal) {
  int aux = errno;
  if (signal == SIGUSR1)
    printf("Signal SIGUSR1 (%d)\n", signal);
  else if (signal == SIGQUIT)
    printf("Captured SIGQUIT signal (sent by PID: %d). Use SIGINT to terminate application.\n", signal);
  errno = aux;
}