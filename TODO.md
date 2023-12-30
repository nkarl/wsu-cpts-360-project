
## High Priority

- [ ] Add support for both EXT2 and EXT4.
- [x] Review the code Reading and Showing `inode_table` and `root_node`.
    - [ ] Find a way to write more deterministic tests for them.
        - The tests could try to address the case where `ino` exceeds the total number of inodes on disk.
        - The tests could try to address the case where `ino` for the `root_node` is 1.

## Lower Priority

- [ ] Implement an EXT4 version of EXT2 struct.
- [ ] Implement `FS::Write` for `DIR_ENTRY`.
- [ ] Peruse KC's book on the next components of the FS.
- [ ] Plan out the *first set of function* that interfaces with `FS::EXT2`.
    - mkdir
    - creat
- [ ] review the data type model.

## Done

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

