#include "../hdr/type.h"
#include <stdlib.h>

/**********************************************************************
 * start up files
 **********************************************************************/
#include "cd_ls_pwd.c"
#include "util.c"
/*#include "globals.c"*/  // included in cd_ls_pwd

/**********************************************************************
 * function signatures
 **********************************************************************/
int init();
int show_dir(MINODE *mip);
int hit_ratio();
int quit();

/**********************************************************************
 * INITiALIZE the FS.
 */
int init() {
    /**
     * initialize (in-memory) inodes into a freeList.
     * creates a linked list of free minodes.
     */
    for (int i = 0; i < NUM_MINODE; i++) {
        MINODE *mip = &minode[i];
        mip->dev = mip->ino = 0;
        mip->id             = i;
        mip->next           = &minode[i + 1];
    }
    minode[NUM_MINODE - 1].next = 0;
    freeList                    = &minode[0];  // free minodes list
    cacheList                   = 0;           // cacheList = 0

    /**
     * set all oft to FREE.
     */
    for (int i = 0; i < NUM_OFT; i++)
        oft[i].shareCount = 0;

    /**
     * intialize all procs.
     * We have only 2 processes. P1 is always SUPER user then?
     */
    for (int i = 0; i < NUM_PROC; i++) {
        PROC *p = &proc[i];
        p->uid = p->gid = i;      // uid=0 for SUPER user
        p->pid          = i + 1;  // pid = 1,2,..., NUM_PROC-1

        // for each process, open all file descriptors
        for (int j = 0; j < NUM_FD; j++)
            p->fd[j] = 0;  // open file descritors are 0
    }

    running  = &proc[0];  // P1 is running
    requests = hits = 0;  // for hit_ratio of minodes cache

    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * show_dir
 */
int show_dir(MINODE *mip) {
    // show contents of mip DIR: same as in LAB5
    INODE *ip = &(mip->INODE);
    char   sbuf[BLOCK_SIZE], temp[256];
    DIR   *d_ptr;
    char  *cp;

    // ASSUME only one data block i_block[0]
    // YOU SHOULD print i_block[0] number here
    printf("i_block[0]=%d", ip->i_block[0]);
    get_block(dev, ip->i_block[0], sbuf);

    d_ptr = (DIR *)sbuf;
    cp    = sbuf;

    while (cp < (sbuf + BLOCK_SIZE)) {
        strncpy(temp, d_ptr->name, d_ptr->name_len);
        temp[d_ptr->name_len] = 0;

        printf("%4d %4d %4d %s\n",
               d_ptr->inode,
               d_ptr->rec_len,
               d_ptr->name_len,
               temp);

        cp += d_ptr->rec_len;
        d_ptr = (DIR *)cp;
    }
    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * get cache hit ratio
 */
int hit_ratio() {
    // print cacheList;
    // compute and print hit_ratio
    return 100 * hits / requests;
}

/**********************************************************************
 * quit FS
 */
int quit() {
    MINODE *mip = cacheList;
    while (mip) {
        if (mip->shareCount) {
            mip->shareCount = 1;
            iput(mip);  // write INODE back if modified
        }
        mip = mip->next;
    }
    exit(EXIT_SUCCESS);
}

/**********************************************************************
 * MAIN ENTRY
 */
int main(int argc, char *argv[]) {
    char line[128];
    char buf[BLOCK_SIZE];

    /**
     * calls init() to set up FS
     */
    init();

    /**
     * open and check dev
     *
     * https://man7.org/linux/man-pages/man2/open.2.html
     */
    fd = dev = open(disk, O_RDWR);
    printf("dev = %d\n", dev);  // YOU should check dev value: exit if < 0
    if (dev < 0) {
        printf("NOT a valid device\n");
        exit(EXIT_FAILURE);
    }

    /**
     * get super block of dev
     */
    get_block(dev, SUPER_BLOCK, buf);
    SUPER *sp = (SUPER *)buf;  // you should check s_magic for EXT2 FS
    if (sp->s_magic != MAGIC_EXT2) {
        printf("NOT an EXT2 FS\n");
        exit(EXIT_FAILURE);
    }

    count_inodes = sp->s_inodes_count;
    count_blocks = sp->s_blocks_count;
    printf("count_inodes=%d  count_blocks=%d\n", count_inodes, count_blocks);

    /**
     * get group descriptor
     */
    get_block(dev, GD_BLOCK, buf);
    GD *gp = (GD *)buf;

    bmap = gp->bg_block_bitmap;
    imap = gp->bg_inode_bitmap;
    iblk = inodes_start = gp->bg_inode_table;  // sets the start of inode table
    printf("bmap=%d  imap=%d  iblk=%d\n", bmap, imap, iblk);

    // HERE =========================================================
    MINODE *mip = freeList;  // remove minode[0] from freeList
    freeList    = freeList->next;
    cacheList   = mip;  // enter minode[0] in cacheList

    // get root INODE
    get_block(dev, iblk, buf);
    INODE *ip  = (INODE *)buf + 1;  // #2 INODE
    mip->INODE = *ip;               // copy into mip->INODE

    mip->cacheCount = 1;
    mip->shareCount = 2;  // for root AND CWD
    mip->modified   = 0;

    root = mip;  // root points at #2 INODE in minode[0]

    printf("set P1's CWD to root\n");
    running->cwd = root;  // CWD = root
    /*
    // not running properly
    root         = iget(dev, ROOT_INODE);
    running->cwd = iget(dev, ROOT_INODE);
    */
    // END_HERE =====================================================

    /********* write code for iget()/iput() in util.c **********
             Replace code between HERE and ENDhere with

     root         = iget(dev, 2);
     running->cwd = iget(dev, 2);
    **********************************************************/

    const int RUNNING = 1;
    while (RUNNING) {
        printf("P%d running\n", running->pid);
        pathname[0] = parameter[0] = 0;

        printf("enter command [cd|ls|pwd|exit] : ");
        fgets(line, 128, stdin);
        line[strlen(line) - 1] = 0;  // kill \n at end

        if (line[0] == 0)
            continue;

        sscanf(line, "%s %s %64c", cmd, pathname, parameter);
        printf("pathname=%s parameter=%s\n", pathname, parameter);

        if (strcmp(cmd, "ls") == 0)
            ls();
        if (strcmp(cmd, "cd") == 0)
            cd();
        if (strcmp(cmd, "pwd") == 0)
            pwd();

        if (strcmp(cmd, "show") == 0)
            show_dir(mip);
        if (strcmp(cmd, "hits") == 0)
            hit_ratio();
        if (strcmp(cmd, "exit") == 0)
            quit();
    }
    // placeholder return
    return EXIT_SUCCESS;
}
