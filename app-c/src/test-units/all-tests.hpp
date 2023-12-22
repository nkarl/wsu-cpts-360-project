#ifndef ALL_TESTS_HPP
#define ALL_TESTS_HPP

#include <stdio.h>
#include <string>

struct Test {

    void Header(std::string header) {
        printf("- %s", header.c_str());
    }

    void Body(void (*test)()) {
        test();
    }

    void Footer() {
        printf("%50c [x]", 0);
    }
};

void test_fs_ext2();

#endif
