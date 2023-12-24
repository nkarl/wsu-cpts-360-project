#include "../hdr-locked/my-types.hpp"

#include "../test-units/all-tests.hpp"

#include <stdio.h>

int main() {
    printf("\n\nRUNNING TESTS.\n\n");
    //test_fs_ext2();
    test_fs_ext2_imap();

    printf("\n\nALL: PASSED.\n");
}
