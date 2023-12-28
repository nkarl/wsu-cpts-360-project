
### High Priority

- [x] Implement `FS::Read` and `FS::Show` for the root node.
    - NOTE:
        - at the moment the root node is empty without a record.
        - will need to set a record for the root node via `DIR_ENTRY`.
- [ ] Implement `FS::Read` and `FS::Show` for `DIR_ENTRY`.
    - [ ] Implement `FS::Write` for `DIR_ENTRY`.

### Lower Priority

- [ ] Peruse KC's book on the next components of the FS.
- [ ] Plan out the *first set of function* that interfaces with `FS::EXT2`.
    - mkdir
    - creat
- [ ] review the data type model.
