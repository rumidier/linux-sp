#include <stdio.h>
#include <pwd.h>
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
extern int ls_control_opt_a;
extern int ls_control_opt_l;
char **ls_control_get_files = NULL;
char **ls_control_get_dirs = NULL;


void
ls_control_get_file (int     len,
                     char   *file)
{
  ls_control_get_files = (char **) realloc (ls_control_get_files, (len + 2) * sizeof (char *));
  ls_control_get_files[len] = file;
  ls_control_get_files[len + 1] = NULL;
}

void
ls_control_get_dir (int     len,
                    char   *dir)
{
  ls_control_get_dirs = (char **) realloc (ls_control_get_dirs, (len + 2) * sizeof (char *));
  ls_control_get_dirs[len] = dir;
  ls_control_get_dirs[len + 1] = NULL;
}

void
ls_control_file (char **files)
{
  int i;
  struct stat sb;

  if (files == NULL)
    {
      return;
    }
  else{
    char mode[10];
    int uid_len;
    int gid_len;
    int link_len;
    int link_max;
    int size_len;
    int size_max;
    unsigned long user;

    link_len = 0;
    size_len = 0;
    link_max = 0;
    size_max = 0;
    uid_len = 0;
    gid_len = 0;

    if (ls_control_opt_l)
      {

        for (i = 0; files[i] != NULL;++i)
          {
            if (stat(files[i], &sb) == -1)
              {
                perror("stat");
                exit(EXIT_FAILURE);
              }

            uid_len = ls_control_uid_len (uid_len, files[i]);
            gid_len = ls_control_gid_len (gid_len, files[i]);
            (link_max < sb.st_nlink) ? (link_max = sb.st_nlink) : (link_max);
            (size_max < sb.st_size) ? (size_max = sb.st_size) : (size_max);
          }
        link_len = ls_control_get_len (link_max);
        size_len = ls_control_get_len (size_max);

        for (i = 0; files[i] != NULL; ++i)
          {
            if (stat(files[i], &sb) == -1)
              {
                perror("stat");
                exit(EXIT_FAILURE);
              }

            printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
            user = sb.st_mode & 00700;
            printf ("user ---> %lo\n", user);

            user = user >> 6;
            printf ("user ---> %lo\n", user);


            printf("Link count:               %ld\n", (long) sb.st_nlink);
            printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);

            printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
            printf("File size:                %lld bytes\n", (long long) sb.st_size);

            printf("Last file modification:   %s", ctime(&sb.st_mtime));
          }
      }
    else
      {
      }
  }
}

int
ls_control_get_len (int max_num)
{
  int i;
  int count;
  int buf_num;

  i = 1;
  count = 0;
  while (1)
    {
      buf_num = max_num / i;
      if (buf_num <= 0)
        {
          break;
        }
      i *= 10;
      count++;
    }

  return count;
}

int
ls_control_uid_len (int   len,
                    char *file)
{
  struct passwd *uid = NULL;
  uid = getpwuid (1000);

  printf ("%d : %s, %s, %s\n", 1000, uid->pw_name, uid->pw_uid, uid->pw_gid);

  return 0;
}

int
ls_control_gid_len (int   len,
                    char *file)
{
  return 0;
}
