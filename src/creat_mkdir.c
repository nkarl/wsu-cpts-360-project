#include "../hdr/creat_mkdir.h"
#include "../hdr/util.h"
#include <time.h>

extern PROC *running;
extern int   dev;
extern int   fd, dev;

extern MINODE *root;

int mk_dir(char *pathname) {
    /*
    1. Let
        parent = dirname(pathname);   parent= "/a/b" OR "a/b"
        child  = basename(pathname);  child = "c"

        WARNING: strtok(), dirname(), basename() destroy pathname
    */
    char  cbuf[512] = {0};
    char *parent, *child;
    strcpy(cbuf, pathname);
    parent = dirname(cbuf);
    strcpy(cbuf, pathname);
    child = basename(cbuf);
    printf("\t> dirname: %s \tbasename: %s\n", parent, child);

    /*
    2. Get minode of parent:
        MINODE *pip = path2inode(parent);
        (print message if pip NULL; return -1 for error)
    */
    MINODE *pip = path2inode(parent);
    if (!pip) {
        printf("> ERROR: in mk_dir(%s), MINODE not found.\n", parent);
        return -1;
    }

    /*
    3. Verify : (1). parent INODE is a DIR (HOW?)   AND
                (2). child does NOT exists in the parent directory (HOW?);
    */
    if (!S_ISDIR(pip->INODE.i_mode)) {  // check DIR type
        printf("parent: %s is not a directory\n", parent);
        return 0;
    }

    int child_ino = search(pip, child);
    if (!child_ino) {
        printf("child: %s does not exist in parent: %s\n", parent, child);
        return 0;
    }
    /*
    4. call mymkdir(pip, child);
    */
    my_mk_dir(pip, child);

    /*
    5. inc parent inodes's links count by 1;
        touch its atime, i.e. atime = time(0L), mark it modified
    */
    pip->INODE.i_atime = time(0L);
    pip->modified      = 1;

    /*
    6. iput(pip);
    */
    iput(pip);
    return 0;
}

/*
1. pip points at the parent minode[] of "/a/b", name is a string "c"
*/
int my_mk_dir(MINODE *pip, char *name) {
    int ino,
        bno;
    /*
    2. allocate an inode and a disk block for the new directory;
            ino = ialloc(dev);
            bno = balloc(dev);
       Don't WORK IN THE DARK: PRINT OUT THESE NUMBERS!!!
    */
    ino = ialloc(dev);
    bno = balloc(dev);
    printf("\t> in my_mk_dir: ino=%d  bno=%d", ino, bno);

    /*
    3. MINODE *mip = iget(dev, ino); //load inode into a minode[] (in order to
       wirte contents to the INODE in memory.
    */

    /*
    4. Write contents to mip->INODE to make it a DIR INODE. Mark it modified;
    */

    /*
    5. iput(mip); which writes the new INODE out to disk.

      // C CODE of (3), (4) and (5):
      **********************************************************************
      MINDOE *mip = iget(dev,ino);
      INODE *ip = &mip->INODE;
      Use ip-> to acess the INODE fields:

      i_mode = 0x41ED;		// OR 040755: DIR type and permissions
      i_uid  = running->uid;	// Owner uid
      i_gid  = running->gid;	// Group Id
      i_size = BLKSIZE;		// Size in bytes
      i_links_count = 2;	        // Links count=2 because of . and ..
      i_atime = i_ctime = i_mtime = time(0L);  // set to current time
      i_blocks = 2;                	// LINUX: Blocks count in 512-byte chunks
      i_block[0] = bno;             // new DIR has one data block

      i_block[1] to i_block[14] = 0;

      mip->modified = 1;            // mark minode MODIFIED
      iput(mip);                    // write INODE to disk
    */
    MINODE *mip = iget(dev, ino);
    INODE  *ip  = &mip->INODE;

    ip->i_mode        = 0x41ED;        // OR 040755: DIR type and permissions
    ip->i_uid         = running->uid;  // Owner uid
    ip->i_gid         = running->gid;  // Group Id
    ip->i_size        = BLOCK_SIZE;    // Size in bytes
    ip->i_links_count = 2;             // Links count=2 because of . and ..

    ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);  // set to current time

    ip->i_blocks   = 2;    // LINUX: Blocks count in 512-byte chunks
    ip->i_block[0] = bno;  // new DIR has one data block
                           // initialize all data entries
    for (int i = 1; i < 15; ++i) {
        ip->i_block[i] = 0;
    }

    mip->modified = 1;  // sets modified
    iput(mip);
    /*
    // ***** create data block for new DIR containing . and .. entries ******
    6. Write . and .. entries to a buf[ ] of BLKSIZE

       | entry .     | entry ..     |                                       |
       ----------------------------------------------------------------------
       |ino|12|1|.   |pino|1012|2|..                                        |
       ----------------------------------------------------------------------

       Then, write buf[ ] to the disk block bno;
    */
    char dbuf[BLOCK_SIZE] = {0};  // empty upon new created dir

    DIR  *dp = (DIR *)dbuf;
    char *cp;

    // make self entry "."
    dp->inode    = ino;
    dp->rec_len  = 12;
    dp->name_len = 1;
    dp->name[0]  = '.';

    // make parent entry ".."
    int pino = pip->ino;
    int blk  = bno;

    cp = dbuf + 12;
    dp = (DIR *)cp;

    dp->inode    = pip->ino;
    dp->rec_len  = BLOCK_SIZE - 12;
    dp->name_len = 2;
    dp->name[0] = dp->name[1] = '.';

    put_block(dev, blk, dbuf);
    /*
    7. Finally, enter name ENTRY into parent's directory by
                enter_child(pip, ino, name);
    */
    enter_child(pip, ino, name);
    return 0;
}

int enter_child(MINODE *pip, int ino, char *name) {
    int NEED_LEN = 4 * ((8 + strlen(name) + 3) / 4);
    int i;
    for (i = 0; i < 12; i++) {
        if (pip->INODE.i_block[i] == 0) break;
    }
    char buf[BLOCK_SIZE] = {0};
    get_block(pip->dev, pip->INODE.i_block[i - 1], buf);
    DIR  *dp = (DIR *)buf;
    char *cp = buf;

    int IDEAL_LEN = 4 * ((8 + dp->name_len + 3) / 4);
    while (cp < buf + BLOCK_SIZE) {
        cp += dp->rec_len;
        dp = (DIR *)cp;
    }
    int REMAIN = dp->rec_len - IDEAL_LEN;
    if (REMAIN >= NEED_LEN) {
        dp->rec_len = IDEAL_LEN;
        cp += dp->rec_len;
        dp           = (DIR *)cp;
        dp->inode    = ino;
        dp->name_len = strlen(name);
        strncpy(dp->name, name, dp->name_len);
        dp->rec_len = REMAIN;
    }
    else {
    }
    return 0;
}
