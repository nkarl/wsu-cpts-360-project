#include "../hdr/util.h"
#include "globals.c"

/**********************************************************************
 * get_block
 */
int get_block(int dev, int blk, char buf[]) {
    lseek(dev, blk * BLOCK_SIZE, SEEK_SET);
    /**
     * https://man7.org/linux/man-pages/man2/read.2.html
     */
    int n = read(fd, buf, BLOCK_SIZE);
    return n;
}

/**********************************************************************
 * put_block
 */
int put_block(int dev, int blk, char buf[]) {
    lseek(dev, blk * BLOCK_SIZE, SEEK_SET);
    int n = write(fd, buf, BLOCK_SIZE);
    return n;
}

/**********************************************************************
 * tokenize pathname
 */
int tokenize(char *pathname) {
    // tokenize pathname into n token strings in (global) gline[ ]
    strcpy(gline, pathname);
    char *s = strtok(gline, "/");
    int   i = 0;
    do {
        name[i++]   = s;
        amount_name = i;
        s           = strtok(0, "/");
    } while (s);

    // placeholder return
    return EXIT_SUCCESS;
}

void print_mip(MINODE *mip) {
    if (mip) {
        printf("\t\t\t> mip->INODE=%ld\n", (size_t)&mip->INODE);
        printf("\t\t\t> mip->dev=%d  mip->ino=%d\n", mip->dev, mip->ino);
        printf("\t\t\t> mip->cacheCount=%d\n", mip->cacheCount);
        printf("\t\t\t> mip->shareCount=%d\n", mip->shareCount);
        printf("\t\t\t> mip->modified=%d\n", mip->modified);
        printf("\t\t\t> mip->id=%d\n", mip->id);
        printf("\t\t\t> mip->next=%ld\n", (size_t)mip->next);
    }
}

/**********************************************************************
 * return a pointer to some minode in dev with given ino.
 */
MINODE *iget(int dev, int ino)  // return minode pointer of (dev, ino)
{
    printf("\t >> ENTER iget()\n");
    MINODE *mip, *l_cacheList = cacheList, *l_freeList = freeList;
    /*MTABLE *mp;*/  // not yet needed for level 1
    INODE *ip;
    int    block, offset;
    char   buf[BLOCK_SIZE];

    requests++;

    /********** Write code to implement these ************/

    /* 1. search cacheList for minode=(dev, ino):
    if (found) {
        inc minode's cacheCount by 1;
        inc minode's shareCount by 1;
        return minode pointer;
    }*/
    mip = l_cacheList;
    while (mip) {
        if (mip->shareCount && (mip->dev == dev) && (mip->ino == ino)) {  // if exists at least one ref and ...
            mip->cacheCount++;
            mip->shareCount++;
            hits++;
            return mip;
        }
        mip = mip->next;
    }
    printf("\t >> DONE checking in cacheList.\n");
    // needed (dev, ino) NOT in cacheList.

    /* 2.
    if (freeList NOT empty){
        remove a minode from freeList;
        set minode to (dev, ino), cacheCount=1 shareCount=1, modified=0;

        load INODE of (dev, ino) from disk into minode.INODE;

        enter minode into cacheList;
        return minode pointer;
    }*/
    printf("\t >> BEFORE if (freeList).\n");
    if (l_freeList) {
        printf("\t >> ENTERED if (l_freeList).\n");
        /*
         * if there is a free minode, load inode with given ino into it.
         */
        mip        = l_freeList;
        l_freeList = l_freeList->next;

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
    printf("\t >> DONE checking in freeList.\n");

    // freeList empty case:
    /*
    3. find a minode in cacheList with shareCount=0, cacheCount=SMALLest
       set minode to (dev, ino), shareCount=1, cacheCount=1, modified=0
       return minode pointer;

   NOTE: in order to do 3:
         it's better to order cacheList by INCREASING cacheCount,
         with smaller cacheCount in front ==> search cacheList
    ************/
    mip = (MINODE *)searchCacheList();
    if (mip) {
        mip->dev        = dev;
        mip->ino        = ino;
        mip->shareCount = 1;
        mip->cacheCount = 1;
        mip->modified   = 0;
        return mip;
    }
}

/**********************************************************************
 * search cache
 */
size_t searchCacheList() {
    MINODE *curr = freeList;

    while (curr->shareCount > 0) {
        if (curr->shareCount == 0) {
            hits++;
            return (size_t)curr;
        }
        curr = curr->next;
    }
    return (size_t)curr;
}

/**********************************************************************
 * add to cache
 * TODO: THIS IS BROKEN. DEBUG.
 */
void insertCacheList(MINODE *mip) {
    MINODE *curr = cacheList;
    if (!curr) {
        cacheList = mip;
        mip->next = 0;
        return;
    }

    if ((mip->shareCount = 0) && (mip->cacheCount <= curr->cacheCount)) {
        mip->next = curr;
        cacheList = mip;
        return;
    }

    MINODE *prev = curr;
    while (prev->cacheCount >= curr->cacheCount) {
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

/**********************************************************************
 * put inode
 */
void iput(MINODE *mip)  // release a mip
{
    INODE *ip;
    char   buf[BLOCK_SIZE];
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
    printf("block=%d offset=%d", block, offset);

    get_block(mip->dev, block, buf);
    ip  = (INODE *)buf + offset;
    *ip = mip->INODE;
    put_block(mip->dev, block, buf);
    mip->shareCount = 0;
    return;
}

/**********************************************************************
 * search mip by dir name
 */
int search(MINODE *mip, char *name) {
    /******************
    search the data blocks of some mip->INODE for the given name:
    if (found) return its inode number;
    else       return 0;
    ******************/
    char *cp, temp[BLOCK_SIZE], sbuf[BLOCK_SIZE];
    DIR  *dp;
    for (int i = 0; i < 12; i++) {  // search DIR direct blocks only
        printf("\t> i=%d\n", i);
        if (mip->INODE.i_block[i] == 0)
            return EXIT_SUCCESS;
        get_block(mip->dev, mip->INODE.i_block[i], sbuf);
        dp = (DIR *)sbuf;
        cp = sbuf;
        while (cp < sbuf + BLOCK_SIZE) {
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
    return EXIT_SUCCESS;
}

/**********************************************************************
 * search by pathname
 */
MINODE *path2inode(char *pathname) {
    /*******************
    return minode pointer of pathname;
    return 0 if pathname invalid;

    This is same as YOUR main loop in LAB5 without printing block numbers
    *******************/
    MINODE *mip;
    int     i, ino;
    if (strcmp(pathname, "/") == 0) {
        return root;
        // return root ino=2
    }
    // if absolute pathname: start from root
    if (pathname[0] == '/') {
        mip = root;
    }
    // if relative pathname: start from CWD
    else {
        mip = running->cwd;
    }
    mip->shareCount++;
    // in order to iput(mip) later
    tokenize(pathname);
    // assume: name[ ], amount_name are globals
    for (i = 0; i < amount_name; i++) {
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
    // return the mip with ino=ino
    /*return ino;*/
    return mip;
}

/**********************************************************************
 * search for ino by pathname
 */
int findmyname(MINODE *pip, int myino, char myname[]) {
    /****************
    pip points to parent DIR minode:
    search for myino;    // same as search(pip, name) but search by ino
    copy name string into myname[256]
    ******************/
    int   i;
    char *cp, /*temp[256],*/ sbuf[BLOCK_SIZE];
    DIR  *dp;
    for (i = 0; i < 12; i++) {  // search DIR direct blocks only
        if (pip->INODE.i_block[i] == 0)
            return 0;
        get_block(pip->dev, pip->INODE.i_block[i], sbuf);
        dp = (DIR *)sbuf;
        cp = sbuf;
        while (cp < sbuf + BLOCK_SIZE) {
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

/**********************************************************************
 * search for ino by pathname
 */
int findino(MINODE *mip, int *myino) {
    /*****************
    mip points at a DIR minode
    i_block[0] contains .  and  ..
    get myino of .
    return parent_ino of ..
    *******************/
    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * report error
 */
int error(char *s, int exit_code) {
    fprintf(stderr, "%s\n", s);
    return exit_code;
}

/**********************************************************************
 * test bits in the bitmap of buf
 */
int tst_bit(char *buf, int bit) {
    return buf[bit / 8] & (1 << (bit % 8));
}

/**********************************************************************
 * set the bits in the bitmap of buf
 */
int set_bit(char *buf, int bit) {
    buf[bit / 8] |= (1 << (bit % 8));
    return EXIT_SUCCESS;
}

/**********************************************************************
 * decrease the amount of free inodes from dev
 */
void decFreeInodes(int dev) {
    char buf[BLOCK_SIZE];

    // dec free inodes count by 1 in SUPER and GD
    get_block(dev, 1, buf);
    SUPER *sp = (SUPER *)buf;
    sp->s_free_inodes_count--;
    put_block(dev, 1, buf);

    get_block(dev, 2, buf);
    GD *gp = (GD *)buf;
    gp->bg_free_inodes_count--;
    put_block(dev, 2, buf);
}

/**********************************************************************
 * allocate an inode number from imap block
 */
int ialloc(int dev) {
    char buf[BLOCK_SIZE];

    // read inode_bitmap block
    get_block(dev, imap, buf);

    for (int i = 0; i < amount_inodes; i++) {
        if (tst_bit(buf, i) == 0) {
            set_bit(buf, i);
            put_block(dev, imap, buf);

            decFreeInodes(dev);

            printf("ialloc : ino=%d\n", i + 1);
            return i + 1;
        }
    }
    return 0;
}

/**********************************************************************
 * decrease the amount of free blocks from dev
 */
void decFreeBlocks(int dev) {
    char buf[BLOCK_SIZE];

    // dec free blocks count by 1 in SUPER and GD
    get_block(dev, 1, buf);
    SUPER *sp = (SUPER *)buf;
    sp->s_free_blocks_count--;
    put_block(dev, 1, buf);

    get_block(dev, 2, buf);
    GD *gp = (GD *)buf;
    gp->bg_free_blocks_count--;
    put_block(dev, 2, buf);
}

/**********************************************************************
 * allocate a block number from bmap block
 */
int balloc(int dev) {
    char buf[BLOCK_SIZE];

    // read inode_bitmap block
    get_block(dev, bmap, buf);

    for (int i = 0; i < amount_blocks; i++) {
        if (tst_bit(buf, i) == 0) {
            set_bit(buf, i);
            put_block(dev, bmap, buf);

            decFreeBlocks(dev);

            printf("balloc : ino=%d\n", i + 1);
            return i + 1;
        }
    }
    return 0;
}
