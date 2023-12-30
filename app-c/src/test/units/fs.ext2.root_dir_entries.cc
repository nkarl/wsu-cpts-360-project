#include "../../hdr-locked/constants.hpp"
#include "../../hdr-locked/my-fs.hpp"
#include "../../hdr-locked/my-types.hpp"

#include "all-tests.hpp"

#include <cassert>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2_root_dir_entries() {
    /**
     * SET UP
     */
    []() -> void {
        //string create_vdisk{
            //"if [ ! -f vdisk ]; then "
            //"WRITE_LIMIT=1024;"
            //"INODE_SIZE=128;"
            //"dd if=/dev/zero of=vdisk bs=$WRITE_LIMIT count=1440;"
            //"mke2fs -c vdisk -I $INODE_SIZE -b $WRITE_LIMIT;"
            //"fi"};

        //system(create_vdisk.c_str());
    }();

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
        FS::Read::EXT2::root_node(&vdisk);
        FS::Show::EXT2::root_node(&vdisk);
    }

    /**
     * TEAR DOWN
     */
    [=]()
        -> void {
        // string remove_vdisk{
        //"echo cleaning up the test...;"
        //"if [ -f vdisk ]; then "
        //"rm vdisk;"
        //"fi"
        //};
        // system(remove_vdisk.c_str());
    }();
    return true;
}

/**
 * test executor.
 */
void test_fs_ext2_root_dir_entries() {
    Test::Header("show root node dir entries");
    bool result = Test::Body(*_test_fs_ext2_root_dir_entries);
    Test::Footer(result);
}
