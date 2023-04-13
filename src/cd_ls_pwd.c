#include "../hdr/cd_ls_pwd.h"
#include "../hdr/util.h"
#include <time.h>

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
    MINODE *mip = path2inode(pathname);
    print_mip(mip);

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
    char   sbuf[BLOCK_SIZE];
    DIR   *dp;
    char  *cp;
    INODE *ip = &(mip->INODE);

    printf("simple ls_dir()\n");

    /*get_block(dev, pip->INODE.i_block[0], sbuf);*/
    get_block(dev, ip->i_block[0], sbuf);
    dp = (DIR *)sbuf;
    cp = sbuf;

    while (cp < sbuf + BLOCK_SIZE) {
        strncpy(name, dp->name, dp->name_len);
        name[dp->name_len] = 0;
        print_i_mode(ip);
        print_d_entry_stats(ip, dp);
        printf("\t");
        printf("%s\n", name);

        cp += dp->rec_len;
        dp = (DIR *)cp;
    }
    // placeholder return
    return EXIT_SUCCESS;
}

void print_i_mode(INODE *ip) {
    char *t1 = "xwrxwrxwr-------";
    char *t2 = "----------------";

    if ((ip->i_mode & 0xF000) == 0x8000)  // if (S_ISREG())
        printf("%c", '-');
    if ((ip->i_mode & 0xF000) == 0x4000)  // if (S_ISDIR())
        printf("%c", 'd');
    if ((ip->i_mode & 0xF000) == 0xA000)  // if (S_ISLNK())
        printf("%c", 'l');
    for (int i = 8; i >= 0; i--) {
        if (ip->i_mode & (1 << i))
            printf("%c", t1[i]);  // print r|w|x printf("%c", t1[i]);
        else
            printf("%c", t2[i]);  // or print -
    }
}

void print_d_entry_stats(INODE *ip, DIR *e) {
    char   linkname[256];
    char   ftime[64];
    char   fname[256];
    time_t t = ip->i_ctime;
    strcpy(fname, e->name);
    printf("%4d ", ip->i_links_count);  // link count
    printf("%4d ", ip->i_gid);          // gid
    printf("%4d ", ip->i_uid);          // uid
    printf("   ");
    // print time
    strcpy(ftime, ctime(&t));      // print time in calendar form ftime[strlen(ftime)-1] = 0; // kill \n at end
    ftime[strlen(ftime) - 1] = 0;  // removes the \n
    printf("%s ", ftime);          // prints the time
    printf("%8d ", ip->i_size);    // file size
    // print name
    if ((ip->i_mode & 0xF000) == 0xA000) {
        int _ = readlink(fname, linkname, 256);
        printf(" -> %s", linkname);  // print linked name }
    }
}

/**********************************************************************
 * ls_dir
 */
int ls_dir(MINODE *pip) {
    char   sbuf[BLOCK_SIZE], name[256];
    DIR   *dp;
    char  *cp;
    INODE *ip = &(pip->INODE);

    printf("simple ls_dir()\n");
    /*get_block(dev, pip->INODE.i_block[0], sbuf);*/
    get_block(dev, ip->i_block[0], sbuf);
    dp = (DIR *)sbuf;
    cp = sbuf;

    if (strcmp(dp->name, "/") == 0) {
        name[dp->name_len] = 0;
        print_i_mode(ip);
        print_d_entry_stats(ip, dp);
        printf("\t");
        printf("%s\n", name);
        return EXIT_SUCCESS;
    }

    while (cp < sbuf + BLOCK_SIZE) {
        strncpy(name, dp->name, dp->name_len);
        name[dp->name_len] = 0;
        print_i_mode(ip);
        print_d_entry_stats(ip, dp);
        printf("\t");
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

    if (running->cwd) {
        ls_dir(mip);
        return 0;
    }
    /*if (((mip->INODE.i_mode) & 0040000) != 0040000) {*/
    /*ls_file(mip);*/
    /*}*/
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
