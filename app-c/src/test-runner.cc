#include "hdr-locked/my-types.hpp"
#include "tests/sample.arithmetic-operations.cc"
#include "tests/fs.ext2-validation.cc"

void test_runner() {
    test_boot_compile_link();
    test_fs_ext2_super_magic_number();
    test_fs_ext2_super_log_block_number();
    test_fs_ext2();
}
