#include "../hdr-locked/my-fs.hpp"
#include "../hdr-locked/my-types.hpp"

#include "tests.hpp"

#include <cassert>
#include <iostream>

using std::string;

/**
 * This is a vacuous test, but it will be future reference when we actually need mocking.
 */
void test_fs_ext2_super_magic_number() {
    /*
     * 1. mock the FS::EXT2 struct
     * 2. read the super_block and assert on the magic_number
     */
    assert((void("\nsuper magic_number"), 1 == 1));
}

/**
 * This is a vacuous test, but it will be future reference when we actually need mocking.
 */
void test_fs_ext2_super_log_block_number() {
    /*
     * 1. mock the FS::EXT2 struct
     * 2. read the super_block and assert on the log_block_number
     */
    assert((void("\nsuper log_block_number"), 1 == 1));
}

/**
 * Integration test. create a new disk and read from it.
 */
void test_fs_ext2() {
    /**
     * SETUP
     */
    []() -> void {
        string create_test_disk{
            "WRITE_LIMIT=1024;"
            "dd if=/dev/zero of=test_disk bs=$WRITE_LIMIT count=1440;"
            "mke2fs -c test_disk -b 1440;"};

        system(create_test_disk.c_str());
    }();

    auto check_test_disk = []() -> void {
        string check_test_disk{
            "if [ -f test_disk ]; then "
            "echo test_disk created successfully.;"
            "else "
            "echo test_disk does not exist.;"
            "fi"};
        system(check_test_disk.c_str());
    };

    {
        /**
         *
         * TODO:
         *  - [ ] write test to open and read from the test_disk
         *  - [ ] assert magic_number
         *  - [ ] assert log_block_number reduces to 1024 blocksize
         *
         */
    }

        /**
         * CLEANUP
         */
        [=]()
            ->void {
        string remove_test_disk{
            "echo cleaning up the test...;"
            "if [ -f test_disk ]; then "
            "rm test_disk;"
            "fi"};
        system(remove_test_disk.c_str());
        check_test_disk();
    }
    ();
}
