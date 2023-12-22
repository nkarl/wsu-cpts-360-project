#ifndef ALL_TESTS_HPP
#define ALL_TESTS_HPP

#include <stdio.h>
#include <string>

struct Test {

    void Header(std::string header) {
        printf("- %s", header.c_str());
    }

    bool Body(bool (*test)()) {
        return test();
    }

    void Footer(bool result) {
        printf(result ? "%50c [x]" : "%50c [ ]", 0);
    }
};

void test_fs_ext2();

#endif
