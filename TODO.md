
## High Priority

- [ ] Implement `FS::Read` and `FS::Show` for `DIR_ENTRY`.
    - [x] Implement test.
    - BUG: The while loop in `FS::Show::EXT2::root_node` doesn't terminate.
    - FIX:
        - [ ] show details of the first dir entry record. (using printf)
        - [ ] identify the break condition(s).

## Lower Priority

- [ ] Implement `FS::Write` for `DIR_ENTRY`.
- [ ] Peruse KC's book on the next components of the FS.
- [ ] Plan out the *first set of function* that interfaces with `FS::EXT2`.
    - mkdir
    - creat
- [ ] review the data type model.

## Done

