#ifndef ALL_TESTS_HPP
#define ALL_TESTS_HPP

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
        printf("\n%s", std::string(100, '-').c_str());
        printf("[%c]", result ? 'x' : ' ');
    }
};

void test_fs_ext2();
void test_fs_ext2_imap();

#endif
