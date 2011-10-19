#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "ls-control.h"

extern char **ls_control_get_files;
extern char **is_control_get_dirs;

int ls_control_opt_a;
int ls_control_opt_l;

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

static void
ls_main_init (int    argc,
              char **argv)
{
  int opt;

  while ((opt = getopt (argc, argv, "al")) != -1)
    {
      switch (opt)
        {
        case 'l':
          ls_control_opt_l = 1;
          break;
        case 'a':
          ls_control_opt_a = 1;
          break;
        case '?':
            {
              fprintf (stderr, "cannot reconize option\n");
              exit (EXIT_FAILURE);
            }
        default:
            {
              fprintf (stderr, "더 많은 정보를 보려면 `ls --help' 하십시오.\n");
              exit (EXIT_FAILURE);
            }
        }
    }
}

static char **
ls_main_argv_get (int    argc,
             char **argv)
{
  int i;
  int count;
  char **dirs = NULL;

  count = 0;
  for (i = 1; argv[i] != NULL; ++i)
    {
      if (argv[i][0] != '-')
        {
          dirs = (char **) realloc (dirs, (count + 2) * sizeof (char *));
          dirs[count] = argv[i];
          dirs[count + 1] = NULL;
          count++;
        }
    }

  return dirs;
}

int
main (int   argc,
      char *argv[])
{
  int i;
  int file_count = 0;
  int dir_count = 0;
  char **get_argv;

  /**
   * - do not print getopt's error message
   * - process command line argument
   */
  opterr = 0;
  ls_main_init (argc, argv);

  get_argv = ls_main_argv_get (argc, argv);

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
            ls_control_get_file (file_count, get_argv[i]);
            ++file_count;
          }
          else if (S_ISDIR(sbuf.st_mode)) {
            ls_control_get_dir (dir_count, get_argv[i]);
            ++dir_count;
          }
        }
    }
  else
    {
      ls_control_get_dir (dir_count, ".");
      ++dir_count;
    }

  ls_control_file (ls_control_get_files);

  return 0;
}
