#ifndef _LS_CONTROL_H
#define _LS_CONTROL_H

int
ls_opt_get (int     argc,
            char    **argv);

char **
ls_argv_get (int     argc,
             char    **argv);

void
ls_get_file (int     len,
             char    *file);

void
ls_get_dir (int     len,
            char    *dir);

void
ls_file (char **files);

#endif
