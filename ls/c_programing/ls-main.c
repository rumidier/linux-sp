#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "ls-control.h"

extern char **get_files;
extern char **get_dirs;


/**
 *  If an error was detected, and the first character
 *  of optstring is not a colon, and the external variable
 *  opterr  is nonzero (which is the default),
 *  getopt() prints an error message.
 *
 *  If  getopt() does not recognize an option character,
 *  it prints an error message to stderr,
 *  stores the character in  optopt, and  returns  '?'.
 *  The calling program may prevent the error message
 *  by setting opterr to 0.
 */

int
main (int   argc,
      char *argv[])
{
  int i;
  int opt_value;
  int file_count = 0;
  int dir_count = 0;
  char **get_argv;
  /**
   * do not print getopt's error message
   */
  opt_value = ls_opt_get (argc, argv);
  get_argv = ls_argv_get (argc, argv);

  struct stat sbuf;

  if (get_argv != NULL)
    {
      for (i = 0; get_argv[i] != NULL; ++i)
        {
          if (lstat (get_argv[i], &sbuf) == -1)
            {
              fprintf (stderr, "ls: %s에 접근할 수 없습니다.: 그런 파일이나 디렉터리가 없습니다\n", get_argv[i]);
            }

          if (S_ISREG(sbuf.st_mode)) {
            ls_get_file (file_count, get_argv[i]);
            ++file_count;
          }
          else if (S_ISDIR(sbuf.st_mode)) {
            ls_get_dir (dir_count, get_argv[i]);
            ++dir_count;
          }
        }
    }
  else
    {
      ls_get_dir (dir_count, ".");
      ++dir_count;
    }

  ls_file (get_files);

  return 0;
}
