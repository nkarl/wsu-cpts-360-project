#ifndef ALL_TESTS_HH
#define ALL_TESTS_HH

#include <stdio.h>
#include <string>

struct Test {

    static void Header(std::string header) {
        printf("\nTEST: %s.", header.c_str());
        printf("\n%s", std::string(100, '-').c_str());
    }

    static bool Body(bool (*test)()) {
        return test();
    }

    static void Footer(bool result) {
        printf("\n%s", std::string(100, '.').c_str());
        printf("[%c]\n", result ? 'x' : ' ');
    }
};

void test_fs_ext2();
void test_fs_ext2_imap();
void test_fs_ext2_imap_bits();
void test_fs_ext2_inode_table();
void test_fs_ext2_root_dir_entries();
void test_fs_ext2_utils();

#endif
