#ifndef MY_FS_HPP
#define MY_FS_HPP

#include "my-types.hpp"

#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

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
        i32         first_inode_num;

        i8 *super_block      = nullptr;
        i8 *group_desc_block = nullptr;
        i8 *imap             = nullptr;
        i8 *inode_table      = nullptr;
        /*
         * TODO: could be useful to implement a stack for dir_entry here.
         */

        EXT2(i8 const *const device_name) : device_name(device_name) {
            fd = open(device_name, O_RDONLY);
            if (fd < 0) {
                printf("open %sfailed\n", device_name);
                exit(1);
            }

            this->super_block      = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
            this->group_desc_block = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
            this->imap             = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
            this->inode_table      = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
        }

        ~EXT2() {
            delete super_block;
            delete group_desc_block;
            delete imap;
            delete inode_table;
        }
    };

    namespace Read {
        struct EXT2 {
            /**
             * read the block in to an i8 buffer
             */
            static size_t read_block(i32 fd, u32 block_num, i8 *buffer) {
                lseek(fd, block_num * constants::BASE_BLOCK_SIZE, SEEK_SET);
                return read(fd, buffer, constants::BASE_BLOCK_SIZE);
            }

            /**
             * reads information from the inode table.
             */
            static void inode_table(FS::EXT2 *ext2) {
                if (!ext2->group_desc_block && ext2->inode_table) {
                    return;
                }
                GD       *gdp       = (GD *)(ext2->group_desc_block);
                const u32 block_num = gdp->bg_inode_table;

                read_block(ext2->fd, block_num, ext2->inode_table);
            }

            /**
             * reads information from the IMAP block.
             */
            static void imap(FS::EXT2 *ext2) {
                if (!ext2->super_block && ext2->group_desc_block) {
                    return;
                }
                GD       *gdp       = (GD *)(ext2->group_desc_block);
                const u32 block_num = gdp->bg_inode_bitmap;

                read_block(ext2->fd, block_num, ext2->imap);
            }

            /**
             * reads disk information from the SUPER block.
             */
            static SUPER *super(FS::EXT2 *ext2) {
                const u32 fd          = ext2->fd;
                const u32 block_num   = 1;
                i8       *super_block = ext2->super_block;

                // lseek(fd, blksize * block_num, SEEK_SET);
                // read(fd, super_block, blksize);
                read_block(fd, block_num, super_block);
                SUPER *sp = (SUPER *)super_block;

                //  as a super_block block structure, check EXT2 FS magic number:
                if (sp->s_magic != constants::MAGIC_NUMBER) {
                    printf("NOT an EXT2 FS\n");
                    exit(2);
                }
                // printf("EXT2 FS OK\n");

                ext2->blksize         = constants::BASE_BLOCK_SIZE * (1 << sp->s_log_block_size);
                ext2->first_inode_num = sp->s_first_ino;
                return sp;
            }

            /**
             * reads group information from the GD block.
             */
            static GD *group_desc(FS::EXT2 *ext2) {
                const u32 fd               = ext2->fd;  //, blksize = ext2->blksize;
                const u32 block_num        = 2;
                i8       *group_desc_block = ext2->group_desc_block;
                // lseek(fd, blksize * block_num, SEEK_SET);
                // read(fd, group_desc_block, blksize);
                read_block(fd, block_num, group_desc_block);
                GD *gdp = (GD *)group_desc_block;
                return gdp;
            }
        };
    }  // namespace Read

    namespace Show {
        struct EXT2 {
            static void dir_entry(FS::EXT2 const *const ext2) {
                /*
                 * TODO: implement the algorithm to step through the dir entries (`dir_entries`) in an inode's data block.
                 *  - get data block buffer.
                 *  - jump forward by the record's length of each dir_entry.
                 */
                i8 buf[constants::BASE_BLOCK_SIZE], record_name[256];

                auto iTABLE = 20;
                FS::Read::EXT2::read_block(ext2->fd, iTABLE, buf);

                i8 *rp = buf;  // record pointer

                DIR_ENTRY *dep = (DIR_ENTRY *)buf;

                /*
                 * BUG: currently doesn't show the correct inode number and dir entries.
                 * TODO: write test for this.
                 */
                // while (rp < buf + constants::BASE_BLOCK_SIZE) {
                std::strncpy(record_name, dep->name, dep->name_len);
                record_name[dep->name_len] = 0;
                // std::string record_name = std::string(dep->name, dep->rec_len);
                printf("inode[%d] %d %d %s\t", dep->inode, dep->rec_len, dep->name_len, record_name);
                rp += dep->rec_len;
                dep = (DIR_ENTRY *)rp;
                //}
            }

            static void inode_table(FS::EXT2 const *const ext2) {
                INODE *ip = (INODE *)ext2->inode_table;
                ++ip;
                printf("\nmode = %4x ", ip->i_mode);
                printf("\nuid = %d gid = %d", ip->i_uid, ip->i_gid);
                printf("\nsize = %d", ip->i_size);
                printf("\nctime = %s", std::ctime((i64 *)&ip->i_ctime));
                printf("\nlinks = %d\n", ip->i_links_count);
                for (u32 i = 0; i < 15; i++) {
                    // print disk block numbers
                    if (ip->i_block[i])
                        // print non-zero blocks only
                        printf("i_block[%d] = %d\n", i, ip->i_block[i]);
                }
                printf("\n");
            }

            /**
             * build a bitmap from the byte passed in.
             */
            static u8 *byte2bitstring(u8 value) {
                u8 *bitstring = (u8 *)malloc(sizeof(u8) * 8);
                for (u8 i = 0; i < 8; ++i) {
                    bitstring[i] = (value & static_cast<u8>(1 << (i % 8))) >> (i % 8);
                }
                return bitstring;
            }

            /**
             * print a bit string.
             */
            static void print_bitstring(u8 *bitstring) {
                for (u32 j = 0; j < 8; ++j) {
                    // printf("%s", bitstring); // this won't work because 1 != '1' in ASCII.
                    printf("%1d", bitstring[j]);
                }
                printf(" ");
            }

            /**
             * show the bitmap of all inodes on ext2.
             */
            static void imap(FS::EXT2 const *const ext2) {
                SUPER *sp         = (SUPER *)(ext2->super_block);
                u32    num_inodes = sp->s_inodes_count;

                i8 *imap = ext2->imap;
                printf("\nimap: as 8-bit stringss\n");
                for (u32 i = 0; i <= num_inodes / 8; ++i) {
                    if (i % 4 == 0) {
                        printf("\n");
                    }
                    printf("\tbytes[%3d]  %02x  ", i + 1, (u8)imap[i]);
                    u8 *bitstring = byte2bitstring((u8)imap[i]);
                    print_bitstring(bitstring);
                    delete bitstring;
                }
            }

            /**
             * show the hexmap of all inodes on ext2.
             *
             * NOTE:
             *  - Memory is allocated in chunks. This is done automatically as an optimization by the computer.
             *      - https://www.c-faq.com/struct/align.html
             *  - `Each byte` is represented as `2 hex values`, effectively compressing the amount of data from 8 points to 2 points.
             *      - e.g. `0xff <- 0x11111111`
             *      - This encoding scheme is an example of `symbolic abstraction`.
             *      - We use a symbolic value (hex) to represent a larger amount of information (bits).
             *          - We have to read less but still extract the same amount of meaning from the heap of information.
             *          - When we need to, we can decode the hex values and expand them into a `bitmap` or bitstring.
             */
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

    namespace Write {
        struct EXT2 {
            static u8 set_bit(u8 byte, u8 index) {
                return byte | (u8)(1 << index);
            }

            static u8 clear_bit(u8 byte, u8 index) {
                return byte ^ (u8)(1 << index);
            }
        };
    }  // namespace Write
};     // namespace FS
#endif
