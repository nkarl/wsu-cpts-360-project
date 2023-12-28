#include "../hdr-locked/constants.hpp"
#include "../hdr-locked/my-fs.hpp"
#include "../hdr-locked/my-types.hpp"

#include "all-tests.hpp"

#include <cassert>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2_inode_table() {
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

        FS::Read::EXT2::super(&vdisk);
        FS::Show::EXT2::super(&vdisk);
        FS::Read::EXT2::group_desc(&vdisk);
        FS::Show::EXT2::group_desc(&vdisk);
        FS::Read::EXT2::inode_table(&vdisk);
        FS::Show::EXT2::inode_table(&vdisk);
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
void test_fs_ext2_inode_table() {
    Test::Header("show inode table");
    bool result = Test::Body(*_test_fs_ext2_inode_table);
    Test::Footer(result);
}
