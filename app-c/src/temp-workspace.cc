#include "hdr/my-types.hpp"

#include <ctime>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hdr/constants.hpp"
#include "hdr/ext2fs.hpp"

namespace FileSystem {
    struct EXT2 {
        typedef struct ext2_block_super      SUPER;
        typedef struct ext2_block_group_desc GD;
        typedef struct ext2_inode            INODE;
        typedef struct ext2_dir_entry_2      DIR_ENTRY;

        u8     fd;
        u16    blksize = constants::BASE_BLOCK_SIZE;
        u32    inodesize;
        SUPER *sp;

        /**
         *
         * reader disk information from the SUPER block.
         *
         */
        void readSUPER(i8 *device) {
            fd = open(device, O_RDONLY);
            if (fd < 0) {
                printf("open %sfailed\n", device);
                exit(1);
            }

            i8 buf[constants::BASE_BLOCK_SIZE];

            lseek(fd, blksize * 1, SEEK_SET);  // block 0 on FD, offset by blksize on HD
            read(fd, buf, blksize);
            sp = (SUPER *)buf;
            // as a super block structure
            // check EXT2 FS magic number:
            printf("%-30s = %8x ", "s_magic", sp->s_magic);
            if (sp->s_magic != 0xEF53) {
                printf("NOT an EXT2 FS\n");
                exit(2);
            }
            printf("EXT2 FS OK\n");
            print("s_inodes_count", sp->s_inodes_count);
            print("s_blocks_count", sp->s_blocks_count);
            print("s_r_blocks_count", sp->s_r_blocks_count);
            print("s_free_inodes_count", sp->s_free_inodes_count);
            print("s_free_blocks_count", sp->s_free_blocks_count);

            print("s_first_data_block", sp->s_first_data_block);
            print("s_log_block_size", sp->s_log_block_size);

            print("s_blocks_per_group", sp->s_blocks_per_group);
            print("s_inodes_per_group", sp->s_inodes_per_group);

            print("s_mnt_count", sp->s_mnt_count);
            print("s_max_mnt_count", sp->s_max_mnt_count);

            printf("%-30s = %8x\n", "s_magic", sp->s_magic);

            printf("%c", '\n');
            print("s_mtime", std::ctime((i64 *)&sp->s_mtime));
            print("s_wtime", std::ctime((i64 *)&sp->s_wtime));

            blksize = constants::BASE_BLOCK_SIZE * (1 << sp->s_log_block_size);
            print("block size", blksize);
            print("inode size", sp->s_inode_size);
        }

        void print(i8 const *s, u32 x) {
            printf("%-30s = %8d\n", s, x);
        }

        void print(i8 const *s, i8 const *x) {
            printf("%-30s = %8s\n", s, x);
        }
    };
}  // namespace FileSystem
