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
// #include "ext2fs.hpp"

#include "../hdr/utils.hpp"
#include "constants.hpp"

namespace FS {
    /**
     * This is an in-memory interface for EXT2 filesystem.
     *
     * NOTE:
     *  - https://github.com/torvalds/linux/blob/master/fs/ext2/ext2.h
     *  - https://ext4.wiki.kernel.org/index.php/Ext4_Disk_Layout#Blocks
     */
    struct EXT2 {
        using SUPER     = ext2_super_block;
        using GD        = ext2_group_desc;
        using INODE     = ext2_inode;
        using DIR_ENTRY = ext2_dir_entry_2;

        std::string device_name;
        i32         fd      = -1;
        u32         blksize = constants::BASE_BLOCK_SIZE;
        u32         first_ino;

        i8 *super_block      = nullptr;
        i8 *group_desc_block = nullptr;
        i8 *imap             = nullptr;
        i8 *inode_table      = nullptr;
        i8 *root_node        = nullptr;
        /*
         * TODO: could be useful to implement a stack for dir_entry in the wrapper data structure (always live while the program running) that holds the state of EXT2.
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
            this->root_node        = (i8 *)malloc(sizeof(i8) * constants::BASE_BLOCK_SIZE);
        }

        ~EXT2() {
            delete super_block;
            delete group_desc_block;
            delete imap;
            delete inode_table;
            delete root_node;
        }
    };

    namespace Read {
        struct EXT2 {
            /**
             * read the block in to an i8 buffer.
             */
            static size_t read_block(i32 fd, u32 block_num, i8 *buffer) {
                lseek(fd, block_num * constants::BASE_BLOCK_SIZE, SEEK_SET);
                return read(fd, buffer, constants::BASE_BLOCK_SIZE);
            }

            /**
             * read the entries of the root node.
             */
            static void root_node(FS::EXT2 *ext2) {
                FS::EXT2::INODE *ip = (FS::EXT2::INODE *)(ext2->inode_table);
                ++ip;
                const u32 block_num = ip->i_block[0];
                read_block(ext2->fd, block_num, ext2->root_node);
            }

            /**
             * reads information from the inode table.
             */
            static void inode_table(FS::EXT2 *ext2) {
                if (!ext2->group_desc_block && !ext2->inode_table) {
                    return;
                }
                const u32 block_num = ext2->first_ino - 1;
                read_block(ext2->fd, block_num, ext2->inode_table);
            }

            /**
             * reads information from the IMAP block.
             */
            static void imap(FS::EXT2 *ext2) {
                if (!ext2->super_block && !ext2->group_desc_block) {
                    return;
                }
                FS::EXT2::GD *gdp       = (FS::EXT2::GD *)(ext2->group_desc_block);
                const u32     block_num = gdp->bg_inode_bitmap;
                read_block(ext2->fd, block_num, ext2->imap);
            }

            /**
             * reads group information from the GD block.
             */
            static FS::EXT2::GD *group_desc(FS::EXT2 *ext2) {
                const u32 fd               = ext2->fd;  //, blksize = ext2->blksize;
                const u32 block_num        = constants::GD_BLOCK;
                i8       *group_desc_block = ext2->group_desc_block;
                read_block(fd, block_num, group_desc_block);
                FS::EXT2::GD *gdp = (FS::EXT2::GD *)group_desc_block;
                return gdp;
            }

            /**
             * reads disk information from the SUPER block.
             */
            static FS::EXT2::SUPER *super(FS::EXT2 *ext2) {
                const u32 fd          = ext2->fd;
                const u32 block_num   = constants::SUPER_BLOCK;
                i8       *super_block = ext2->super_block;
                read_block(fd, block_num, super_block);
                FS::EXT2::SUPER *sp = (FS::EXT2::SUPER *)super_block;

                //  as a super_block block structure, check EXT2 FS magic number:
                if (sp->s_magic != constants::MAGIC_NUMBER) {
                    printf("NOT an EXT2 FS\n");
                    exit(2);
                }
                // printf("EXT2 FS OK\n");

                ext2->blksize   = constants::BASE_BLOCK_SIZE * (1 << sp->s_log_block_size);
                ext2->first_ino = sp->s_first_ino;
                return sp;
            }
        };
    }  // namespace Read

    namespace Show {
        struct EXT2 {
            static void root_node(FS::EXT2 const *const ext2) {
                i8                  *rp = ext2->root_node;  // record pointer
                FS::EXT2::DIR_ENTRY *dp = (FS::EXT2::DIR_ENTRY *)ext2->root_node;
                printf("\n%8s %8s %8s %10s\n", "inode#", "rec_len", "name_len", "rec_name");

                u32 i = 0;  // sets a counter to print only the first ten records
                while (i < 7 && rp < ext2->root_node + constants::BASE_BLOCK_SIZE) {
                    /*
                     * BUG: still doesn't fix the problem with zero-length records.
                     * - The loop never breaks if `rp` starts at a smaller value and is always added by 0 `rec_len`.
                     */
                    std::string record_name = std::string(dp->name, dp->rec_len);
                    printf("%8d %8d %8d %10s\n", dp->inode, dp->rec_len, dp->name_len, record_name.c_str());
                    rp += dp->rec_len;
                    if (dp->rec_len == 0) {
                        /*
                         * FIX: this is a temporary fix.
                         *  - I am not sure yet how records are organized in memory. Is it sparsed or contiguous?
                         *      - If it is the latter, then we can simply break when encountering 0 `rec_len`.
                         *      - This fix covers both cases.
                         *  - What happens after a record is deleted?
                         *     - What happens to the memory space that previously occupied by it?
                         *     - Is it now available for write? How does that work if there is a new rec with larger length we want to write into disk?
                         */
                        rp += 1;
                    }
                    dp = (FS::EXT2::DIR_ENTRY *)rp;
                    ++i;
                }
                if (i == 7) {
                    printf("  . . . omitted remaining entries . . .");
                }
            }

            static void inode_table(FS::EXT2 const *const ext2) {
                FS::EXT2::INODE *ip = (FS::EXT2::INODE *)ext2->inode_table;
                ++ip;
                printf("\nmode  = %x", ip->i_mode);
                printf("\nuid   = %d", ip->i_uid);
                printf("\ngid   = %d", ip->i_gid);
                printf("\nsize  = %d", ip->i_size);
                printf("\nctime = %s", std::ctime((i64 *)&ip->i_ctime));
                printf("\nlinks = %d\n", ip->i_links_count);
                for (u32 i = 0; i < 15; i++) {
                    // print disk block numbers
                    if (ip->i_block[i]) {
                        if (i % 3 == 0) {
                            printf("\n");
                        }
                        printf("\ti_block[%2d] = %d \t", i, ip->i_block[i]);  // print non-zero blocks only
                    }
                }
                printf("\n");
            }

            /**
             * build a bitmap from the byte passed in.
             */
            static u8 *byte2bitstring(u8 value) {
                u8 *bitstring = (u8 *)malloc(sizeof(u8) * 8);
                for (u8 i = 0; i < 8; ++i) {
                    bitstring[i] = (value & static_cast<u8>(1 << i)) >> i;
                }
                return bitstring;
            }

            /**
             * print a bit string.
             */
            static void print_bitstring(u8 *bitstring) {
                for (u8 i = 0; i < 8; ++i) {
                    // printf("%s", bitstring); // this won't work because 1 != '1' in ASCII.
                    printf("%1d", bitstring[i]);
                }
                printf(" ");
            }

            /**
             * show the bitmap of all inodes on ext2.
             */
            static void imap(FS::EXT2 const *const ext2) {
                FS::EXT2::SUPER *sp         = (FS::EXT2::SUPER *)(ext2->super_block);
                u32              num_inodes = sp->s_inodes_count;

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
                FS::EXT2::SUPER *sp         = (FS::EXT2::SUPER *)(ext2->super_block);
                u32              num_inodes = sp->s_inodes_count;

                i8 *imap = ext2->imap;
                printf("\nimap: as hex array\n");
                for (u32 i = 0; i <= num_inodes / 8; ++i) {
                    printf("%02x ", (u8)imap[i]);
                }
            }

            /**
             * shows the info of the gd block of the given ext2.
             */
            static void group_desc(FS::EXT2 const *const ext2) {
                FS::EXT2::GD *gdp = (FS::EXT2::GD *)(ext2->group_desc_block);

                printf("\nGD BLOCK\n");
                printf("----------------------------------------------\n");
                print("bg_block_bitmap", gdp->bg_block_bitmap);
                print("bg_inode_bitmap", gdp->bg_inode_bitmap);
                print("bg_inode_table", gdp->bg_inode_table);
                print("bg_free_blocks_count", gdp->bg_free_blocks_count);
                print("bg_free_inodes_count", gdp->bg_free_inodes_count);
                print("bg_used_dirs_count", gdp->bg_used_dirs_count);
                // print("bg_pad", gdp->bg_pad);
                // print("bg_reserved", *gdp->bg_reserved);
                printf("%c", '\n');
            }

            /**
             * shows the info of the super_block block of the given ext2.
             */
            static void super(FS::EXT2 const *const ext2) {
                FS::EXT2::SUPER *sp = (FS::EXT2::SUPER *)(ext2->super_block);

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
                print("inode size", sp->s_inode_size);
                print("first ino", sp->s_first_ino);
                printf("%c", '\n');
            }
        };
    }  // namespace Show

    struct Utils {
        static u8 set_bit(u8 byte, u8 index) {
            return byte | (u8)(1 << index);
        }

        static u8 clear_bit(u8 byte, u8 index) {
            return byte ^ (u8)(1 << index);
        }
    };
};  // namespace FS
#endif
