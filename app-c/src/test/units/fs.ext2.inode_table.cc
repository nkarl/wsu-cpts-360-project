#include "../../hdr-locked/constants.hh"
#include "../../hdr-locked/my-fs.hh"
#include "../../hdr-locked/my-types.hh"

#include "all-tests.hh"

#include <cassert>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2_inode_table() {
    /**
     * SET UP
     */

    /**
     * TEST BODY
     */
    {
        i8 const *const diskname = "vdisk";
        FS::EXT2        vdisk(diskname);

        FS::Read::EXT2::super(&vdisk);
        FS::Read::EXT2::group_desc(&vdisk);
        FS::Read::EXT2::imap(&vdisk);

        FS::Read::EXT2::inode_table(&vdisk);
        FS::Show::EXT2::inode_table(&vdisk);
    }

    /**
     * TEAR DOWN
     */
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
