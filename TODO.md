
### High Priority

- [ ] Implement `FS::Read` and `FS::Show` for the root node.
- [ ] Implement `FS::Read` and `FS::Show` for `DIR_ENTRY`.

### Lower Priority

- [ ] Peruse KC's book on the next components of the FS.
- [ ] Plan out the *first set of function* that interfaces with `FS::EXT2`.
    - mkdir
    - creat

## Done

- [x] Write tests for setting a bit in a byte.
- [x] Write tests for clearing a bit in a byte.
- [x] Write unit tests for reading the super block.
- [x] Write a test build in the Makefile.
- [x] Work on imap.
    - [x] decoding hex to binary with the correct bit order.
    - [x] building a bit map for inodes.
- [x] Write a note on design choice for data types (i8, u8, i32, u32,...) for efficient byte array decoding/encoding
- [x] Write a note on design choice using both `i8 *` and `std::string`

