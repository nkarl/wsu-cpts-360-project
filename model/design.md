
This project is a system with two major components:

1. a Virtual Disk (vdisk)
2. the Shell program that mounts and loops on that vdisk.


## The Virtual Disk

- The virtual disk can be created via a command line (retrieves reference from KC's book).

- The virtual disk needs to be modeled into EX2 structures in C (retrieves references from KC's book).
	- These structures will hold information pertaining to records saved as data blocks on the vdisk.

```sh
# if:input file, of:output file, bs: number of bytes, count: number of blocks
dd if=/dev/zero of=vdisk bs=1024 count=1440
# 1440+0 records in
# 1440+0 records out
# 1474560 bytes (1.5 MB, 1.4 MiB) copied, 0.00197985 s, 745 MB/s

mke2fs vdisk 1440
# Creating filesystem with 1440 1k blocks and 176 inodes
```

```
|     0 |     1 |  2 |  . . . 7 |    8 |    9 |  10  . . . 32 | 33 . . . 1439 | 
|  boot | super | GD | reserved | bmap | imap | inodes blocks | data blocks   |
```

<table>
  <tr>
    <th style='text-align:right'>block:</th>
    <td>0</td>
    <td>1</td>
    <td>2</td>
    <td>3 .. 7</td>
    <td>8</td>
    <td>9</td>
    <td>10 .. 32</td>
    <td>33 .. 1439</td>
  </tr>
  <tr>
    <th style='text-align:right'>name:</th>
    <td>boot</td>
    <td>super</td>
    <td>group descriptor</td>
    <td>reserved</td>
    <td>bmap</td>
    <td>imap</td>
    <td>inode blocks</td>
    <td>data blocks</td>
  </tr>
  <tr>
    <th style='text-align:right'>info:</th>
    <td></td>
    <td>0+1024
      <hr>contains information about the entire filesystem.
    </td>
    <td>&nbsp
      <hr>EXT2 has groups, each 8192 blocks.
    </td>
    <td></td>
    <td>1439 bits
      <hr>maps disk's usage. 0 for FREE, 1 for IN_USE.
    </td>
    <td>184 nodes
      <hr>maps files. 0 is VALID, 1 is INVALID.
    </td>
    <td>128B per inode</td>
    <td></td>
  </tr>
</table>

## The Shell

- The Shell is a runtime loop.
- It will always wait for a commandline from the user, execute and loop back to wait again.

### Shell Design & Implementation

The shell will be implemented iteratively.

The first phase is to provide an API that interface with the vdisk. This API will start with `ls` which maps disk blocks into correct file records. Similar commands will also be grouped into this phase. These commands are independent programs that can nbe executed to get or modify disk blocks on the vdisk.

The second phase is to implement a scheme of process management for some commands. The simplest option is forking a process. More details will be explored later.

Finally, these commands will be imported and used as a library of a shell program.
