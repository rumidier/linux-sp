#ifndef _LS_CONTROL_H
#define _LS_CONTROL_H

void
ls_control_get_file (int      len,
                     char    *file);

void
ls_control_get_dir (int      len,
                    char    *dir);

void
ls_control_file (char **files);

int
ls_control_get_len (int len);

int
ls_control_uid_len (int   len,
                    char *file);

int
ls_control_gid_len (int   len,
                    char *file);

#endif
