#include "../hdr/type.h"

void    print_i_mode(INODE *ip);
void    print_d_entry_stats(INODE *ip, DIR *dp);
int     get_block(int dev, int blk, char buf[]);
int     put_block(int dev, int blk, char buf[]);
MINODE *iget(int dev, int ino);  // return minode pointer of (dev, ino)
int     tokenize(char *pathname);
MINODE *searchCacheList();
void    insertCacheList(MINODE *mip);
void    iput(MINODE *mip);  // release a mip

int     ialloc(int dev);
void    decFreeInodes(int dev);

int     balloc(int dev);
void    decFreeBlocks(int dev);

MINODE *mialloc();
void    midalloc(MINODE *);
int     search(MINODE *mip, char *name);
MINODE *path2inode(char *pathname);
int     findmyname(MINODE *pip, int myino, char myname[]);
int     findino(MINODE *mip, int *myino);

int error(char *s, int exit_code);

int tst_bit(char *buf, int bit);
int set_bit(char *buf, int bit);
