
### High Priority

- [ ] Implement `FS::Read` and `FS::Show` for `DIR_ENTRY`.
    - [x] Implement test.
    - BUG: The while loop in `FS::Show::EXT2::root_node` doesn't terminate.
    - FIX:
        - [ ] identify the break condition(s).

### Lower Priority

- [ ] Implement `FS::Write` for `DIR_ENTRY`.
- [ ] Peruse KC's book on the next components of the FS.
- [ ] Plan out the *first set of function* that interfaces with `FS::EXT2`.
    - mkdir
    - creat
- [ ] review the data type model.

## Done

- [x] Review KC's book p.230 Mount Partitions.
    - [x] create and mount an ext2 vdisk.

- [x] Implement `FS::Read` and `FS::Show` for the root node.
    - BUG: `iblock[0]` doesn't show anything.
    - FIX:
        - [x] ensure I have the chain of `Read` mapped out in correct order.
        - [x] review the `GD` and `INODE` structs.
    - NOTE: `FS::Read::EXT2::inode_table` is correct. The disk created by the automaton script is faulty. Using the course's vdisk shows the disk block.
