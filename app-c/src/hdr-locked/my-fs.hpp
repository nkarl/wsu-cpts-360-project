#ifndef MY_FS_HPP
#define MY_FS_HPP

#include "my-types.hpp"

#include <ctime>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include <assert.h>

#include <ext2fs/ext2_fs.h>

#include "constants.hpp"
// #include "ext2fs.hpp"

#include "../hdr/utils.hpp"

namespace FS {
    using SUPER     = ext2_super_block;
    using GD        = ext2_group_desc;
    using INODE     = ext2_inode;
    using DIR_ENTRY = ext2_dir_entry_2;

    struct EXT2 {
        std::string device_name;
        i32         fd      = -1;
        u32         blksize = constants::BASE_BLOCK_SIZE;
        i32         inodesize;

        i8 *super_block      = nullptr;
        i8 *group_desc_block = nullptr;
        i8 *imap             = nullptr;

        EXT2(i8 const *const device_name) : device_name(device_name) {
            fd = open(device_name, O_RDONLY);
            if (fd < 0) {
                printf("open %sfailed\n", device_name);
                exit(1);
            }

            this->super_block      = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
            this->group_desc_block = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
            this->imap             = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
        }

        ~EXT2() {
            delete super_block;
            delete group_desc_block;
            delete imap;
        }
    };

    namespace Read {
        struct EXT2 {
            /**
             * read the block in to a buffer
             */
            static size_t read_block(i32 fd, u32 block_num, i8 *buffer) {
                lseek(fd, block_num * constants::BASE_BLOCK_SIZE, SEEK_SET);
                return read(fd, buffer, constants::BASE_BLOCK_SIZE);
            }

            /*
             * reads information from the IMAP block.
             */
            static void imap(FS::EXT2 *ext2) {
                if (!ext2->super_block && ext2->group_desc_block) {
                    return;
                }
                GD *gdp            = (GD *)(ext2->group_desc_block);
                i32 imap_block_num = gdp->bg_inode_bitmap;

                read_block(ext2->fd, imap_block_num, ext2->imap);
            }

            /**
             * reads disk information from the SUPER block.
             */
            static SUPER *super(FS::EXT2 *ext2) {
                i32 fd = ext2->fd, blksize = ext2->blksize;
                i8 *super_block = ext2->super_block;
                lseek(fd, blksize * 1, SEEK_SET);
                read(fd, super_block, blksize);
                SUPER *sp = (SUPER *)super_block;

                //  as a super_block block structure, check EXT2 FS magic number:
                if (sp->s_magic != constants::MAGIC_NUMBER) {
                    printf("NOT an EXT2 FS\n");
                    exit(2);
                }
                // printf("EXT2 FS OK\n");

                ext2->blksize = constants::BASE_BLOCK_SIZE * (1 << sp->s_log_block_size);
                return sp;
            }

            /**
             * reads group information from the GD block.
             */
            static void group_desc(FS::EXT2 *ext2) {
                i32 fd = ext2->fd, blksize = ext2->blksize;
                GD *group_desc_block = (GD *)ext2->group_desc_block;
                lseek(fd, blksize * 2, SEEK_SET);
                read(fd, group_desc_block, blksize);
            }
        };
    }  // namespace Read

    namespace Show {
        struct EXT2 {

            static u8 *hex2bitstring(i8 value) {
                u8 *bitstring = (u8 *)malloc(sizeof(u8) * 8);
                for (i8 i = 0; i < 8; ++i) {
                    bitstring[i] = ((1 << (i % 8)) & (value)) >> (i % 8);
                }
                return bitstring;
            }

            /**
             * TODO: magnify the `imap` byte array (23 bytes) and expand it into a bit map (23 * 8 bits).
             *  - [ ] for each hex value, make an i8 bitstring.
             *  - [ ] construct the mailman's algorithm from
             *      - a bit mask
             *      - a bit setting method
             *      - a bit clearing method
             */
            static void imap(FS::EXT2 const *const ext2) {
                SUPER *sp         = (SUPER *)(ext2->super_block);
                u32    num_inodes = sp->s_inodes_count;

                std::string bitmap;

                i8 *imap = ext2->imap;
                printf("\nimap: as bit-string array\n");
                for (u32 i = 0; i <= num_inodes / 8; ++i) {
                    printf("\tbytes[%02d] %02x ", i + 1, (u8)imap[i]);
                    u8 *bitstring = hex2bitstring((u8)imap[i]);
                    for (u32 j = 0; j < 8; ++j) {
                        printf("%b ", bitstring[j]);
                    }
                    printf("\n");
                    delete(bitstring);
                }
            }

            static void imap(FS::EXT2 const *const ext2, const i8 *const) {
                SUPER *sp         = (SUPER *)(ext2->super_block);
                u32    num_inodes = sp->s_inodes_count;

                i8 *imap = ext2->imap;
                printf("\nimap: as hex array\n");
                for (u32 i = 0; i <= num_inodes / 8; ++i) {
                    printf("%02x ", (u8)imap[i]);
                }
            }

            /**
             * shows the info of the super_block block of the given ext2.
             */
            static void super(FS::EXT2 const *const ext2) {
                SUPER *sp = (SUPER *)(ext2->super_block);

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
                print("s_magic", sp->s_magic, "hex");
                print("s_mtime", std::ctime((i64 *)&sp->s_mtime));
                print("s_wtime", std::ctime((i64 *)&sp->s_wtime));
                print("block size", ext2->blksize);
                // print("inode size"          , sp->s_inode_size);
                print("first inode num", sp->s_first_ino);
                printf("%c", '\n');
            }

            /**
             * shows the info of the gd block of the given ext2.
             */
            static void group_desc(FS::EXT2 const *const ext2) {
                GD *gdp = (GD *)(ext2->group_desc_block);

                printf("\nGD BLOCK\n");
                printf("----------------------------------------------\n");
                print("bg_block_bitmap", gdp->bg_block_bitmap);
                print("bg_inode_bitmap", gdp->bg_inode_bitmap);
                print("bg_inode_table", gdp->bg_inode_table);
                print("bg_free_blocks_count", gdp->bg_free_blocks_count);
                print("bg_free_inodes_count", gdp->bg_free_inodes_count);
                print("bg_used_dirs_count", gdp->bg_used_dirs_count);
                // print("bg_pad"            , gdp->bg_pad);
                // print("bg_reserved"       , *gdp->bg_reserved);
                printf("%c", '\n');
            }
        };
    }  // namespace Show
};     // namespace FS
#endif
