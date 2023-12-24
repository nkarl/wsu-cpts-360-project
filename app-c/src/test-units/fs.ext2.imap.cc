#include "../hdr-locked/constants.hpp"
#include "../hdr-locked/my-fs.hpp"
#include "../hdr-locked/my-types.hpp"

#include "all-tests.hpp"

#include <cassert>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2_imap() {
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
        FS::Read::EXT2::group_desc(&vdisk);
        FS::Read::EXT2::imap(&vdisk);
        // show bitmap of inodes.
        FS::Show::EXT2::imap(&vdisk);

        /**
         * test setting a bit in a byte.
         *
         *                    76543210
         * value  =  80d -> 0x01010000
         * bit_index     ->     5
         * result = 112d -> 0x01110000
         */
        u32 value     = 80;
        u32 bit_index = 5;
        u32 expect    = 112;

        u32 result = FS::Show::EXT2::set_bit(value, bit_index);
        assert(result == expect);

        /**
         * test clearing a bit from a byte.
         *
         *                    76543210
         * value  = 112d -> 0x01110000
         * bit_index     ->     5
         * result =  80d -> 0x01010000
         */
        value     = 112;
        bit_index = 5;
        expect    = 80;

        result = FS::Show::EXT2::clear_bit(value, bit_index);
        assert(result == expect);
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
void test_fs_ext2_imap() {
    Test::Header("show imap");
    bool result = Test::Body(*_test_fs_ext2_imap);
    Test::Footer(result);
}
