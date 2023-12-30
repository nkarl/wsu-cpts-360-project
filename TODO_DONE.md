##### 12/30/2023

- [x] Fixed a critical error in `automaton/make-vdisk` where every new test disk is created with the incorrect spec (wrong block and inode sizes).
    - this bug was the reason why nothing is displayed when querying the root inode; it created the wrong byte offset.
- [x] Implement `FS::Read` and `FS::Show` for `DIR_ENTRY`.
    - [x] Implement test.
    - BUG: The while loop in `FS::Show::EXT2::root_node` doesn't terminate.
    - FIX:
        - [x] show details of the first dir entry record. (using printf)
        - [x] identify the break condition(s).
            - the break condition is when the *value* of the record pointer `i8 *rp` is equal to the start of inode table + `BASE_BLOCK_SIZE`.
    - NOTE:
        - the bug was located at `FS::Read::EXT2::root_node`, where we need to increment the `ip` by 1, because inode numbers are 1-based indices.
        - GUESS (could be wrong, will check again later):
            - a pointer is simply the address to the start of a contiguous chunk of memory.
            - when it is cast to another type, the step of pointer arithmetic changes. For example, when we cast an `i8 *rp` to `INODE *ip`, the increment by `sizeof(i8)` will now increment by `sizeof(INODE)`.

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
