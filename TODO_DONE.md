
- [x] Review KC's book p.230 Mount Partitions.
    - [x] create and mount an ext2 vdisk.

- [x] Implement `FS::Read` and `FS::Show` for the root node.
    - BUG: `iblock[0]` doesn't show anything.
    - FIX:
        - [x] ensure I have the chain of `Read` mapped out in correct order.
        - [x] review the `GD` and `INODE` structs.
    - NOTE: `FS::Read::EXT2::inode_table` is correct. The disk created by the automaton script is faulty. Using the course's vdisk shows the disk block.
- [x] Write tests for setting a bit in a byte.
- [x] Write tests for clearing a bit in a byte.
- [x] Write unit tests for reading the super block.
- [x] Write a test build in the Makefile.
- [x] Work on imap.
    - [x] decoding hex to binary with the correct bit order.
    - [x] building a bit map for inodes.
- [x] Write a note on design choice for data types (i8, u8, i32, u32,...) for efficient byte array decoding/encoding
- [x] Write a note on design choice using both `i8 *` and `std::string`
