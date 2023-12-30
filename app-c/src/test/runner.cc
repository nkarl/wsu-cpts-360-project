#include "../hdr-locked/my-types.hpp"

#include "units/all-tests.hpp"

#include <stdio.h>

int main() {
    printf("\n\nRUNNING TESTS.\n\n");
    test_fs_ext2();
    test_fs_ext2_imap();
    test_fs_ext2_imap_bits();
    test_fs_ext2_inode_table();
    test_fs_ext2_root_dir_entries();

    printf("\n\nALL: PASSED.\n");
}
