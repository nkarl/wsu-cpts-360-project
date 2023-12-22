#include "../hdr-locked/constants.hpp"
#include "../hdr-locked/my-fs.hpp"
#include "../hdr-locked/my-types.hpp"

#include "all-tests.hpp"

#include <cassert>
#include <iostream>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2() {
    /**
     * SET UP
     */
    []() -> void {
        string create_test_disk{
            "if [ ! -f test_disk ]; then "
            "WRITE_LIMIT=1024;"
            "dd if=/dev/zero of=test_disk bs=$WRITE_LIMIT count=1440;"
            "mke2fs -c test_disk -b 1440;"
            "fi"};

        system(create_test_disk.c_str());
    }();

    /**
     * TEST BODY
     */
    {
        i8 const *const diskname = "test_disk";
        FS::EXT2        vdisk(diskname);
        FS::SUPER      *sp = FS::Read::EXT2::block_super(&vdisk);

        assert(sp->s_magic == constants::MAGIC_NUMBER);
        assert(sp->s_log_block_size == 0);  // 0 for ext2
    }

    /**
     * TEAR DOWN
     */
    [=]()
        -> void {
        // string remove_test_disk{
        //"echo cleaning up the test...;"
        //"if [ -f test_disk ]; then "
        //"rm test_disk;"
        //"fi"
        //};
        // system(remove_test_disk.c_str());
    }();
    return true;
}

/**
 * test executor.
 */
void test_fs_ext2() {
    struct Test test;
    test.Header("test: confirms EXT2 filesystem.");
    bool result = test.Body(*_test_fs_ext2);
    test.Footer(result);
}
