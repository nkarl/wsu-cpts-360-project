#include "../hdr/creat_mkdir.h"
#include "../hdr/util.h"
#include <time.h>

extern PROC *running;
extern int   dev;
extern int   fd, dev;

extern MINODE *root;

void mk_dir(char *pathname) {
    /*
    1. Let
        parent = dirname(pathname);   parent= "/a/b" OR "a/b"
        child  = basename(pathname);  child = "c"

        WARNING: strtok(), dirname(), basename() destroy pathname

    2. Get minode of parent:

        MINODE *pip = path2inode(parent);
        (print message if pip NULL; return -1 for error)

    3. Verify : (1). parent INODE is a DIR (HOW?)   AND
                (2). child does NOT exists in the parent directory (HOW?);

    4. call mymkdir(pip, child);

    5. inc parent inodes's links count by 1;
        touch its atime, i.e. atime = time(0L), mark it modified

    6. iput(pip);
*/
}

int my_mk_dir(MINODE *pip, char *name) {
    /*
    1. pip points at the parent minode[] of "/a/b", name is a string "c"

    2. allocate an inode and a disk block for the new directory;
            ino = ialloc(dev);
            bno = balloc(dev);
       Don't WORK IN THE DARK: PRINT OUT THESE NUMBERS!!!

    3. MINODE *mip = iget(dev, ino); //load inode into a minode[] (in order to
       wirte contents to the INODE in memory.

    4. Write contents to mip->INODE to make it a DIR INODE. Mark it modified;

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


    //***** create data block for new DIR containing . and .. entries ******
    6. Write . and .. entries to a buf[ ] of BLKSIZE

       | entry .     | entry ..     |                                       |
       ----------------------------------------------------------------------
       |ino|12|1|.   |pino|1012|2|..                                        |
       ----------------------------------------------------------------------

       Then, write buf[ ] to the disk block bno;

    7. Finally, enter name ENTRY into parent's directory by
                enter_child(pip, ino, name);
    */
}
