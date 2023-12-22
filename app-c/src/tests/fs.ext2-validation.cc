#include "../hdr-locked/my-types.hpp"
#include "../hdr-locked/my-fs.hpp"

#include "tests.hpp"

#include <cassert>

void test_fs_ext2_super_magic_number() {
    /*
     * 1. mock the FS::EXT2 struct
     * 2. read the super_block and assert on the magic_number
     */
    assert((void("\nsuper magic_number"), 0 == 1));
}

void test_fs_ext2_super_log_block_number() {
    /*
     * 1. mock the FS::EXT2 struct
     * 2. read the super_block and assert on the log_block_number
     */
    assert((void("\nsuper log_block_number"), 0 == 1));
}

void test_fs_ext2() {
    /*
     * 1. send string as script to bash
     *   - creates a new disk dd
     * 2. open and read from the created disk
     *
     * 3. test on the struct mapped from the created disk
     *
     */
    assert((void("\nsuper magic_number"), 0 == 1));
    assert((void("\nsuper log_block_number"), 0 == 1));
}
