#ifndef MY_FS_HPP
#define MY_FS_HPP

#include "my-types.hpp"

#include <ctime>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "constants.hpp"
#include "ext2fs.hpp"

#include "../hdr/utils.hpp"

namespace FS {
    typedef struct ext2_block_super      SUPER;
    typedef struct ext2_block_group_desc GD;
    typedef struct ext2_inode            INODE;
    typedef struct ext2_dir_entry_2      DIR_ENTRY;

    struct EXT2 {
        i8 *device;
        u32 fd;
        u32 blksize = constants::BASE_BLOCK_SIZE;
        u32 inodesize;
        i8 *super;
        i8 *group_desc;

        EXT2(i8 *device) : device(device) {
            fd = open(device, O_RDONLY);
            if (fd < 0) {
                printf("open %sfailed\n", device);
                exit(1);
            }
            super      = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
            group_desc = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
        }

        ~EXT2() {
            delete super;
            delete group_desc;
        }
    };

    namespace Read {
        struct EXT2 {
            static void inode(/* params */) {
                /*
                 * TODO: implement the logic to read the inode map.
                 */
            }

            /**
             *
             * reads disk information from the SUPER block.
             *
             */
            static void block_super(FS::EXT2 *ext2) {
                u32 fd = ext2->fd, blksize = ext2->blksize;
                i8 *super = ext2->super;
                lseek(fd, blksize * 1, SEEK_SET);
                read(fd, super, blksize);
                SUPER *sp = (SUPER *)super;

                //  as a super block structure, check EXT2 FS magic number:
                if (sp->s_magic != 0xEF53) {
                    printf("NOT an EXT2 FS\n");
                    exit(2);
                }
                printf("EXT2 FS OK\n");

                ext2->blksize = constants::BASE_BLOCK_SIZE * (1 << sp->s_log_block_size);
            }

            /**
             *
             * reads group information from the GD block.
             *
             */
            static void block_group_desc(FS::EXT2 *ext2) {
                u32 fd = ext2->fd, blksize = ext2->blksize;
                GD *group_desc = (GD *)ext2->group_desc;
                lseek(fd, blksize * 2, SEEK_SET);
                read(fd, group_desc, blksize);
            }
        };
    }  // namespace Read

    namespace Show {
        struct EXT2 {
            /**
             *
             * shows the info of the super block of the given ext2.
             *
             */
            static void block_super(FS::EXT2 const *const ext2) {
                SUPER *sp = (SUPER *)(ext2->super);

                printf("\nSUPER BLOCK\n");
                printf("----------------------------------------------\n");
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
                print("s_magic", sp->s_magic);
                print("s_mtime", std::ctime((i64 *)&sp->s_mtime));
                print("s_wtime", std::ctime((i64 *)&sp->s_wtime));
                print("block size", ext2->blksize);
                print("inode size", sp->s_inode_size);
                printf("%c", '\n');
            }

            /**
             *
             * shows the info of the gd block of the given ext2.
             *
             */
            static void block_group_desc(FS::EXT2 const *const ext2) {
                GD *gdp = (GD *)(ext2->group_desc);

                printf("\nGD BLOCK\n");
                printf("----------------------------------------------\n");
                print("bg_block_bitmap", gdp->bg_block_bitmap);
                print("bg_inode_bitmap", gdp->bg_inode_bitmap);
                print("bg_inode_table", gdp->bg_inode_table);
                print("bg_free_blocks_count", gdp->bg_free_blocks_count);
                print("bg_free_inodes_count", gdp->bg_free_inodes_count);
                print("bg_used_dirs_count", gdp->bg_used_dirs_count);
                print("bg_pad", gdp->bg_pad);
                print("bg_reserved", *gdp->bg_reserved);
                printf("%c", '\n');
            }
        };
    }  // namespace Show
};     // namespace FS
#endif
