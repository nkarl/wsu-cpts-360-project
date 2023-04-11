#include "../hdr/type.h"
#include "globals.c"
/*[>********** globals in main.c **********<]*/
/*extern PROC  proc[NPROC];*/
/*extern PROC *running;*/

/*extern MINODE  minode[NMINODE];  // minodes*/
/*extern MINODE *freeList;         // free minodes list*/
/*extern MINODE *cacheList;        // cacheCount minodes list*/

/*extern MINODE *root;*/

/*extern OFT oft[NOFT];*/

/*extern char  gline[256];  // global line hold token strings of pathname*/
/*extern char *name[64];    // token string pointers*/
/*extern int   nname;       // number of token strings*/

/*extern int ninodes, nblocks;*/
/*extern int bmap, imap, inodes_start, iblk;  // bitmap, inodes block numbers*/

/*extern int  fd, dev;*/
/*extern char cmd[16], pathname[128], parameter[128];*/
/*extern int  requests, hits;*/

void insertCacheList(MINODE *mip);
/**************** util.c file **************/

int get_block(int dev, int blk, char buf[]) {
    lseek(dev, blk * BLKSIZE, SEEK_SET);
    /**
     * https://man7.org/linux/man-pages/man2/read.2.html
     */
    int n = read(fd, buf, BLKSIZE);
    return n;
}

int put_block(int dev, int blk, char buf[]) {
    lseek(dev, blk * BLKSIZE, SEEK_SET);
    int n = write(fd, buf, BLKSIZE);
    return n;
}

int tokenize(char *pathname) {
    // tokenize pathname into n token strings in (global) gline[ ]
}

MINODE *iget(int dev, int ino)  // return minode pointer of (dev, ino)
{
    /********** Write code to implement these ***********
    1. search cacheList for minode=(dev, ino);
    if (found){
       inc minode's cacheCount by 1;
       inc minode's shareCount by 1;
       return minode pointer;
    }
    */
    MINODE *mip;
    MTABLE *mp;
    INODE  *ip;
    int     i, block, offset;
    char    buf[BLKSIZE];

    mip = cacheList;
    requests++;
    while (mip) {
        if (mip->shareCount && (mip->dev == dev) && (mip->ino == ino)) {  // if exists at least one ref and ...
            mip->cacheCount++;
            mip->shareCount++;
            hits++;
            return mip;
        }
        mip = mip->next;
    }

    // needed (dev, ino) NOT in cacheList
    /*
    2. if (freeList NOT empty){
          remove a minode from freeList;
          set minode to (dev, ino), cacheCount=1 shareCount=1, modified=0;

          load INODE of (dev, ino) from disk into minode.INODE;

          enter minode into cacheList;
          return minode pointer;
       }*/
    if (freeList) {
        mip      = freeList;
        freeList = freeList->next;

        mip->dev        = dev;
        mip->ino        = ino;
        mip->cacheCount = 1;
        mip->shareCount = 1;
        mip->modified   = 0;

        block  = (ino - 1) / 8 + iblk;
        offset = (ino - 1) % 8;
        get_block(dev, block, buf);

        ip         = (INODE *)buf + offset;
        mip->INODE = *ip;
        insertCacheList(mip);
        return mip;
    }

    // freeList empty case:
    /*
    3. find a minode in cacheList with shareCount=0, cacheCount=SMALLest
       set minode to (dev, ino), shareCount=1, cacheCount=1, modified=0
       return minode pointer;

   NOTE: in order to do 3:
         it's better to order cacheList by INCREASING cacheCount,
         with smaller cacheCount in front ==> search cacheList
    ************/
    mip             = (MINODE *)searchCacheList();
    mip->dev        = dev;
    mip->ino        = ino;
    mip->shareCount = 1;
    mip->cacheCount = 1;
    mip->modified   = 0;
    return mip;
}

int searchCacheList() {
    MINODE *curr = freeList;

    while (curr->shareCount > 0) {
        if (curr->shareCount == 0) {
            hits++;
            return curr;
        }
        curr = curr->next;
    }
    return curr;
}

void insertCacheList(MINODE *mip) {
    MINODE *curr = cacheList;
    if (mip->shareCount = 0 && mip->cacheCount <= curr->cacheCount) {
        mip->next = curr;
        cacheList = mip;
        return;
    }

    MINODE *prev = curr;
    while (mip->cacheCount > curr->cacheCount) {
        prev = curr;
        curr = curr->next;
    }

    while (mip->cacheCount == curr->cacheCount && mip->shareCount > curr->shareCount) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = mip;
    mip->next  = curr;
}

int iput(MINODE *mip)  // release a mip
{
    INODE *ip;
    char   buf[BLKSIZE];
    int    block, offset;
    /*******************
   1.  if (mip==0)                return;

       mip->shareCount--;         // one less user on this minode

       if (mip->shareCount > 0)   return;
       if (!mip->modified)        return;

   2. // last user, INODE modified: MUST write back to disk
      Use Mailman's algorithm to write minode.INODE back to disk)
      // NOTE: minode still in cacheList;
      *****************/
    if (mip == 0) return;
    mip->shareCount--;
    if (mip->shareCount > 0) return;
    if (!mip->modified) return;

    block  = (mip->ino - 1) / 8 + iblk;
    offset = (mip->ino - 1) % 8;

    get_block(mip->dev, block, buf);
    ip  = (INODE *)buf + offset;
    *ip = mip->INODE;
    put_block(mip->dev, block, buf);
    mip->shareCount = 0;
}

int search(MINODE *mip, char *name) {
    /******************
    search mip->INODE data blocks for name:
    if (found) return its inode number;
    else       return 0;
    ******************/
    int   i;
    char *cp, temp[256], sbuf[BLKSIZE];
    DIR  *dp;
    for (i = 0; i < 12; i++) {  // search DIR direct blocks only
        if (mip->INODE.i_block[i] == 0)
            return 0;
        get_block(mip->dev, mip->INODE.i_block[i], sbuf);
        dp = (DIR *)sbuf;
        cp = sbuf;
        while (cp < sbuf + BLKSIZE) {
            strncpy(temp, dp->name, dp->name_len);
            temp[dp->name_len] = 0;
            printf("%8d%8d%8u %s\n",
                   dp->inode,
                   dp->rec_len,
                   dp->name_len,
                   temp);
            if (strcmp(name, temp) == 0) {
                printf("found %s : inumber = %d\n", name, dp->inode);
                return dp->inode;
            }
            cp += dp->rec_len;
            dp = (DIR *)cp;
        }
    }
    return 0;
}

MINODE *path2inode(char *pathname) {
    /*******************
    return minode pointer of pathname;
    return 0 if pathname invalid;

    This is same as YOUR main loop in LAB5 without printing block numbers
    *******************/
    MINODE *mip;
    int     i, ino;
    if (strcmp(pathname, "/") == 0) {
        return 2;
        // return root ino=2
    }
    if (pathname[0] == '/')
        mip = root;
    // if absolute pathname: start from root
    else
        mip = running->cwd;
    // if relative pathname: start from CWD
    mip->shareCount++;
    // in order to iput(mip) later
    tokenize(pathname);
    // assume: name[ ], nname are globals
    for (i = 0; i < nname; i++) {
        // search for each component string
        if (!S_ISDIR(mip->INODE.i_mode)) {  // check DIR type
            printf("%s is not a directory\n", name[i]);
            iput(mip);
            return 0;
        }
        ino = search(mip, name[i]);
        if (!ino) {
            printf("no such component name %s\n", name[i]);
            iput(mip);
            return 0;
        }
        iput(mip);
        // release current minode
        mip = iget(dev, ino);
        // switch to new minode
    }
    iput(mip);
    return ino;
}

int findmyname(MINODE *pip, int myino, char myname[]) {
    /****************
    pip points to parent DIR minode:
    search for myino;    // same as search(pip, name) but search by ino
    copy name string into myname[256]
    ******************/
    int   i;
    char *cp, temp[256], sbuf[BLKSIZE];
    DIR  *dp;
    for (i = 0; i < 12; i++) {  // search DIR direct blocks only
        if (pip->INODE.i_block[i] == 0)
            return 0;
        get_block(pip->dev, pip->INODE.i_block[i], sbuf);
        dp = (DIR *)sbuf;
        cp = sbuf;
        while (cp < sbuf + BLKSIZE) {
            if (dp->inode == myino) {
                printf("found %d : iname = %s\n", myino, dp->name);
                strcpy(myname, dp->name);
                myname[dp->name_len] = 0;
                return 0;
            }
            cp += dp->rec_len;
            dp = (DIR *)cp;
        }
    }
    return 0;
}

int findino(MINODE *mip, int *myino) {
    /*****************
    mip points at a DIR minode
    i_block[0] contains .  and  ..
    get myino of .
    return parent_ino of ..
    *******************/
}
