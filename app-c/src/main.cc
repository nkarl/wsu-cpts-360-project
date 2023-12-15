#include "hdr/ext2fs.hpp"
#include "utils.cc"
#include <ctime>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct ext2_super_block SUPER;

typedef struct _ {
    int    fd;
    int    blksize = 1024;
    int    inodesize;
    char   buf[1024];
    SUPER *sp;
} DiskReader;

void readSuper(char *device, DiskReader &reader);

int main(int argc, char *argv[]) {
    boot_test();

    for (int i = 0; i < argc; ++i) {
        printf("%d %s\n", i, argv[i]);
    }

    char *diskname;
    if (argc >= 1) {
        diskname = argv[1];
    }
    else {
        exit(1);
    }

    DiskReader reader;
    readSuper(diskname, reader);

    return 0;
}

void readSuper(char *device, DiskReader &reader) {
    reader.fd = open(device, O_RDONLY);
    if (reader.fd < 0) {
        printf("open %sfailed\n", device);
        exit(1);
    }
    lseek(reader.fd, (long)1024 * 1, 0);  // block 1 on FD, offset 1024 on HD
    read(reader.fd, reader.buf, 1024);
    reader.sp = (SUPER *)reader.buf;
    // as a super block structure
    // check EXT2 FS magic number:
    printf("%-30s = %8x ", "s_magic", reader.sp->s_magic);
    if (reader.sp->s_magic != 0xEF53) {
        printf("NOT an EXT2 FS\n");
        exit(2);
    }
    printf("EXT2 FS OK\n");
    print("s_inodes_count",
          reader.sp->s_inodes_count);
    print("s_blocks_count",
          reader.sp->s_blocks_count);
    print("s_r_blocks_count",
          reader.sp->s_r_blocks_count);
    print("s_free_inodes_count", reader.sp->s_free_inodes_count);
    print("s_free_blocks_count", reader.sp->s_free_blocks_count);
    print("s_first_data_block", reader.sp->s_first_data_block);
    print("s_log_block_size",
          reader.sp->s_log_block_size);
    print("s_blocks_per_group", reader.sp->s_blocks_per_group);
    print("s_inodes_per_group", reader.sp->s_inodes_per_group);
    print("s_mnt_count",
          reader.sp->s_mnt_count);
    print("s_max_mnt_count",
          reader.sp->s_max_mnt_count);
    printf("%-30s = %8x\n", "s_magic", reader.sp->s_magic);
    printf("s_mtime = %s", std::ctime((long *)&reader.sp->s_mtime));
    printf("s_wtime = %s", std::ctime((long *)&reader.sp->s_wtime));
    reader.blksize = 1024 * (1 << reader.sp->s_log_block_size);
    printf("block size = %d\n", reader.blksize);
    printf("inode size = %d\n", reader.sp->s_inode_size);
}
