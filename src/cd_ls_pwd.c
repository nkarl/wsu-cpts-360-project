#include "../hdr/cd_ls_pwd.h"
#include "../hdr/util.h"

extern PROC *running;
extern int   dev;
extern int   fd, dev;

extern MINODE *root;

// cd_ls_pwd.c file

/**********************************************************************
 * cd
 */
int cd(char *pathname) {
    printf("cd: Under Construction\n");

    // write YOUR code for cd

    /* remember to set bmap and imap for the new cwd */

    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * ls_file
 */
int ls_file(MINODE *mip, char *name) {
    printf("ls_file: under construction\n");

    // use mip->INODE to ls_file
    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * ls_dir
 */
int ls_dir(MINODE *pip) {
    char  sbuf[BLOCK_SIZE], name[256];
    DIR  *dp;
    char *cp;

    printf("simple ls_dir()\n");

    get_block(dev, pip->INODE.i_block[0], sbuf);
    dp = (DIR *)sbuf;
    cp = sbuf;

    while (cp < sbuf + BLOCK_SIZE) {
        strncpy(name, dp->name, dp->name_len);
        name[dp->name_len] = 0;
        printf("%s\n", name);

        cp += dp->rec_len;
        dp = (DIR *)cp;
    }
    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * ls
 */
int ls() {
    MINODE *mip = running->cwd;

    ls_dir(mip);
    iput(mip);
    // placeholder return
    return EXIT_SUCCESS;
}

/**********************************************************************
 * pwd
 */
int rpwd(MINODE *wd) {
    MINODE *temp_root = root;
    if (wd == temp_root) return EXIT_SUCCESS;

    MINODE *mip = wd;
    INODE  *ip  = &(mip->INODE);
    int     ino = search(wd, "..");

    return EXIT_SUCCESS;
}

int pwd(MINODE *mip) {
    printf("CWD = %s\n", "/");
    rpwd(mip);

    // placeholder return
    return EXIT_SUCCESS;
}
