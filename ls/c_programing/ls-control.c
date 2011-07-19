#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#include "ls-control.h"

extern int opterr;
char **get_files = NULL;
char **get_dirs = NULL;

int
ls_opt_get (int argc,
            char **argv)
{
  int opt;
  int opt_value;

  opt_value = 0;
  opterr = 0;
  while ((opt = getopt (argc, argv, "al")) != -1)
    {
      switch (opt)
        {
        case 'l':
            {
              if (opt_value == 2)
                {
                  opt_value = 1; // 1 = l
                }
              else
                {
                  opt_value = 3; //3 = la
                }
              break;
            }
        case 'a':
            {
              if (opt_value == 1)
                {
                  opt_value = 2; // 2 = a
                }
              else
                {
                  opt_value = 3; //3 = la
                }
              break;
            }
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

  return opt_value;
}

char **
ls_argv_get (int    argc,
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

void
ls_get_file (int    len,
             char   *file)
{
  get_files = (char **) realloc (get_files, (len + 2) * sizeof (char *));
  get_files[len] = file;
  get_files[len + 1] = NULL;
}

void
ls_get_dir (int    len,
            char   *dir)
{
  get_dirs = (char **) realloc (get_dirs, (len + 2) * sizeof (char *));
  get_dirs[len] = dir;
  get_dirs[len + 1] = NULL;
}

void
ls_file (char **files)
{
  int i;
  struct stat sb;

  for (i = 0; files[i] != NULL; ++i)
    {
      if (stat(files[i], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
      }
      printf("Mode:                     %lo (octal)\n",
             (unsigned long) sb.st_mode);


      printf("Link count:               %ld\n", (long) sb.st_nlink);
      printf("Ownership:                UID=%ld   GID=%ld\n",
             (long) sb.st_uid, (long) sb.st_gid);

      printf("Preferred I/O block size: %ld bytes\n",
             (long) sb.st_blksize);
      printf("File size:                %lld bytes\n",
             (long long) sb.st_size);

      printf("Last file modification:   %s", ctime(&sb.st_mtime));
    }
}
