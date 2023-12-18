#include "hdr-locked/my-types.hpp"

#include <ctime>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hdr-locked/constants.hpp"
#include "hdr-locked/ext2fs.hpp"

#include "lib/utils.cc"

namespace FileSystem {
    struct EXT2 {
        typedef struct ext2_block_super      SUPER;
        typedef struct ext2_block_group_desc GD;
        typedef struct ext2_inode            INODE;
        typedef struct ext2_dir_entry_2      DIR_ENTRY;

        i8    *device;
        u8     fd;
        u16    blksize = constants::BASE_BLOCK_SIZE;
        u32    inodesize;
        SUPER *sp;
        GD    *gdp;

        EXT2(i8 *device) : device(device) {
            fd = open(device, O_RDONLY);
            if (fd < 0) {
                printf("open %sfailed\n", device);
                exit(1);
            }
        }

        /**
         *
         * reads group information from the GD block.
         *
         */
        void readGD() {
            i8 buf[constants::BASE_BLOCK_SIZE];

            lseek(fd, blksize * 2, SEEK_SET);  // block 0 on FD, offset by blksize on HD
            read(fd, buf, blksize);

            gdp = (GD *)buf;

            print("bg_block_bitmap"      , gdp->bg_block_bitmap);
            print("bg_inode_bitmap"      , gdp->bg_inode_bitmap);
            print("bg_inode_table"       , gdp->bg_inode_table);
            print("bg_free_blocks_count" , gdp->bg_free_blocks_count);
            print("bg_free_inodes_count" , gdp->bg_free_inodes_count);
            print("bg_used_dirs_count"   , gdp->bg_used_dirs_count);
            print("bg_pad"               , gdp->bg_pad);
            print("bg_reserved"          , *gdp->bg_reserved);
        }

        /**
         *
         * reads disk information from the SUPER block.
         *
         */
        void readSUPER() {
            i8 buf[constants::BASE_BLOCK_SIZE];

            lseek(fd, blksize * 1, SEEK_SET);  // block 0 on FD, offset by blksize on HD
            read(fd, buf, blksize);
            sp = (SUPER *)buf;
            // as a super block structure, check EXT2 FS magic number:
            printf("%-30s = %8x ", "s_magic", sp->s_magic);
            if (sp->s_magic != 0xEF53) {
                printf("NOT an EXT2 FS\n");
                exit(2);
            }
            printf("EXT2 FS OK\n");

            blksize = constants::BASE_BLOCK_SIZE * (1 << sp->s_log_block_size);

            print("s_inodes_count"      , sp->s_inodes_count);
            print("s_blocks_count"      , sp->s_blocks_count);
            print("s_r_blocks_count"    , sp->s_r_blocks_count);
            print("s_free_inodes_count" , sp->s_free_inodes_count);
            print("s_free_blocks_count" , sp->s_free_blocks_count);
            print("s_first_data_block"  , sp->s_first_data_block);
            print("s_log_block_size"    , sp->s_log_block_size);
            print("s_blocks_per_group"  , sp->s_blocks_per_group);
            print("s_inodes_per_group"  , sp->s_inodes_per_group);
            print("s_mnt_count"         , sp->s_mnt_count);
            print("s_max_mnt_count"     , sp->s_max_mnt_count);
            print("s_magic"             , sp->s_magic);
            print("s_mtime"             , std::ctime((i64 *)&sp->s_mtime));
            print("s_wtime"             , std::ctime((i64 *)&sp->s_wtime));
            print("block size"          , blksize);
            print("inode size"          , sp->s_inode_size);
            printf("%c", '\n');
        }
    };
}  // namespace FileSystem
