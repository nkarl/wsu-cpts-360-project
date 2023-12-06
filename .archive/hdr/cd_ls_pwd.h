#ifndef CD_LS_PWD_H
#define CD_LS_PWD_H

#include "../hdr/type.h"
#include <stdlib.h>

void cd(char *);
int ls_file(MINODE *mip, char *name);
int ls_dir(MINODE *pip);
int ls();
int pwd();

#endif
