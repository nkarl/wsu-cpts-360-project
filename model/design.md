
This project is a system with two major components:

1. a Virtual Disk (vdisk)
2. the Shell program that mounts and loops on that vdisk.


## The Virtual Disk

- The virtual disk can be created via a command line (retrieves reference from KC's book).

- The virtual disk needs to be modeled into EX2 structures in C (retrieves references from KC's book).
	- These structures will hold information pertaining to records saved as data blocks on the vdisk.

On every virtual disk the boot block (B-00) can be ignored. The relevent information start with B-01, the super block. The super block defines the disk structure. From a high-level view, data on disk is recorded as bytes. Bytes are grouped together as blocks. Blocks are grouped together as groups.

#### The Model of EXT2 File Systems

```txt
- a         disk :
  - a      group : 8192 blocks maximum        , or 8 MB
    - a    block : 1024 bytes                 , or 1 KB
                 :    8 inodes
      - a  inode :  128 bytes
        - a byte :    1 unit of encoded data  ,    8 bits
```

#### Our Working Vdisk
Our working virtual disk is instantiated as follows:

```txt
1                 disk :
  1              group : 1440              blocks  ,             or ~1.4 MB (size of a floppy disk)
    1440        blocks : (1440 - 33) * 1024 bytes  , B[33..1439] or ~1.4 MB of actual space for data
      184       inodes : (  33 - 10) *    8 inodes , B[10..  32]
       23 inode blocks : (  33 - 10)
```

#### Creating Our Working Vdisk

```sh
# if:input file, of:output file, bs: number of bytes, count: number of blocks
dd if=/dev/zero of=vdisk bs=1024 count=1440
# 1440+0 records in
# 1440+0 records out
# 1474560 bytes (1.5 MB, 1.4 MiB) copied, 0.00197985 s, 745 MB/s

mke2fs vdisk 1440
# Creating filesystem with 1440 1k blocks and 176 inodes
```

#### The Layout of the Vdisk

```txt
|    0 |     1 |  2 |  . . . 7 |    8 |    9 |  10  . . . 32 | 33 . . . 1439 | 
| boot | super | GD | reserved | bmap | imap |        inodes |          data |
```

<table>
  <tr>
    <th style='text-align:left'>0</th>
    <th style='text-align:left'>1</th>
    <th style='text-align:left'>2</th>
    <th style='text-align:left'>3..7</th>
    <th style='text-align:left'>8</th>
    <th style='text-align:left'>9</th>
    <th style='text-align:left'>10..32</th>
    <th style='text-align:left'>33..1439</th>
  </tr>
  <tr>
    <td style='background-color:gray'>boot</td>
    <td>super</td>
    <td>GD</td>
    <td style='background-color:gray'>reserved</td>
    <td>bmap</td>
    <td>imap</td>
    <td>inodes</td>
    <td>data</td>
  </tr>
  <tr>
    <td style='background-color:gray'></td>
    <td><code>0+1024 B</code>
      <hr>describes the disk.
    </td>
    <td>&nbsp
      <hr>describes the groups, each 8192 blocks.
    </td>
    <td style='background-color:gray'></td>
    <td><code>1439 b</code>
      <hr>maps disk usage. 0 for FREE, 1 for IN_USE.
    </td>
    <td>184 nodes
      <hr>maps file entries.<br><code>VALID = 0</code><br><code>INVALID = 1</code>
    </td>
    <td><code>128 B</code> per inode</td>
    <td></td>
  </tr>
</table>

## The Disk Interface

This section defines the interface model to get a disk's structural information.

```c
// header definition
#ifndef EXT2_FS_H
#define EXT2_FS_H

#define EXT2_NAME_LEN 256
```

### B-01. the super block

The **super** block holds the definition of the virtual disk. The structure with relevant attributes are defined below.

```c
// typedef u8, u16, u32 SUPER for convenience
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
struct ext2_super_block {
    u32 s_inodes_count;       /* Inodes count */
    u32 s_blocks_count;       /* Blocks count */
    u32 s_r_blocks_count;     /* Reserved blocks count */
    u32 s_free_blocks_count;  /* Free blocks count */
    u32 s_free_inodes_count;  /* Free inodes count */

    u32 s_first_data_block;   /* First Data Block */
    u32 s_log_block_size;     /* Block size */

    u32 s_log_cluster_size;   /* Allocation cluster size */
    u32 s_blocks_per_group;   /* # Blocks per group */
    u32 s_clusters_per_group; /* # Fragments per group */
    u32 s_inodes_per_group;   /* # Inodes per group */
    u32 s_mtime;              /* Mount time */
    u32 s_wtime;              /* Write time */

    u16 s_mnt_count;          /* Mount count */
    u16 s_max_mnt_count;      /* Maximal mount count */

    u16 s_magic;              /* Magic signature */

    // more non-essential fields
    u16 s_inode_size; /* size of inode structure */
};
```
### B-02. the group descriptor block

The *group descriptor* (**GD**) block. 

```c
struct ext2_group_desc {
    u32 bg_block_bitmap;       // Bmap block number
    u32 bg_inode_bitmap;       // Imap block number
    u32 bg_inode_table;        // Inodes begin block number
                               //
    u16 bg_free_blocks_count;  // THESE are OBVIOUS
    u16 bg_free_inodes_count;
    u16 bg_used_dirs_count;
    u16 bg_pad;  // ignore these
    u32 bg_reserved[3];
};
```
**NOTE**: *it is possible that blocks $3..7$ are reserved for more group descriptors*. On systems that are more massive, more GDs will be used.

### B-10. the inode blocks

```c
struct ext2_inode {
    u16 i_mode;   // 16 bits = |tttt|ugs|rwx|rwx|rwx|
    u16 i_uid;    // owner uid
    u32 i_size;   // file size in bytes
    u32 i_atime;  // time fields in seconds
    u32 i_ctime;  // since 00:00:00,1-1-1970
    u32 i_mtime;
    u32 i_dtime;
    u16 i_gid;          // group ID
    u16 i_links_count;  // hard-link count
    u32 i_blocks;       // number of 512-byte sectors
    u32 i_flags;        // IGNORE
    u32 i_reserved1;    // IGNORE

    u32 i_block[15];    // See details below

    u32 i_pad[7];       // for inode size = 128 bytes
};
```

## The Shell

- The Shell is a runtime loop.
- It will always wait for a commandline from the user, execute and loop back to wait again.

### Shell Design & Implementation

The shell will be implemented iteratively.

The first phase is to provide an API that interface with the vdisk. This API will start with `ls` which maps disk blocks into correct file records. Similar commands will also be grouped into this phase. These commands are independent programs that can nbe executed to get or modify disk blocks on the vdisk.

The second phase is to implement a scheme of process management for some commands. The simplest option is forking a process. More details will be explored later.

Finally, these commands will be imported and used as a library of a shell program.
