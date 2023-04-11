#include "../hdr/type.h"

int     get_block(int dev, int blk, char buf[]);
int     put_block(int dev, int blk, char buf[]);
MINODE *iget(int dev, int ino);  // return minode pointer of (dev, ino)
int     tokenize(char *pathname);
size_t  searchCacheList();
void    insertCacheList(MINODE *mip);
void    iput(MINODE *mip);  // release a mip
int     search(MINODE *mip, char *name);
MINODE *path2inode(char *pathname);
int     findmyname(MINODE *pip, int myino, char myname[]);
int     findino(MINODE *mip, int *myino);
