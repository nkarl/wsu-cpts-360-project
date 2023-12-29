
### High Priority

- [x] Review KC's book p.230 Mount Partitions.
    - [x] create and mount an ext2 vdisk.

- [ ] Implement `FS::Read` and `FS::Show` for the root node.
    - BUG: `iblock[0]` doesn't show anything.
    - FIX:
        - ensure I have the chain of `Read` mapped out in correct order.
        - review the `GD` and `INODE` structs.
- [ ] Implement `FS::Read` and `FS::Show` for `DIR_ENTRY`.

### Lower Priority

- [ ] Implement `FS::Write` for `DIR_ENTRY`.
- [ ] Peruse KC's book on the next components of the FS.
- [ ] Plan out the *first set of function* that interfaces with `FS::EXT2`.
    - mkdir
    - creat
- [ ] review the data type model.
