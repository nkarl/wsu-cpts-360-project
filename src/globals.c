#include "../hdr/type.h"

/********** globals **************/
PROC  proc[NPROC];
PROC *running;

MINODE  minode[NMINODE];  // in memory INODES: count=64
MINODE *freeList;         // free minodes list
MINODE *cacheList;        // cached minodes list

MINODE *root;  // root minode pointer

OFT oft[NOFT];  // for level-2 only

char  gline[256];  // global line hold token strings of pathname
char *name[64];    // token string pointers
int   nname;       // number of token strings

int ninodes, nblocks;                // ninodes, nblocks from SUPER block
int bmap, imap, inodes_start, iblk;  // bitmap, inodes block numbers

int  fd, dev;
char cmd[16], pathname[128], parameter[128];
int  requests, hits;
