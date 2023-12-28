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
        string create_vdisk{
            "if [ ! -f vdisk ]; then "
            "WRITE_LIMIT=1024;"
            "dd if=/dev/zero of=vdisk bs=$WRITE_LIMIT count=1440;"
            "mke2fs -c vdisk -b 1440;"
            "fi"};

        system(create_vdisk.c_str());
    }();


    /**
     * TEST BODY
     */
    {
        i8 const *const diskname = "vdisk";
        FS::EXT2        vdisk(diskname);
        FS::SUPER      *sp = FS::Read::EXT2::super(&vdisk);

        assert(sp->s_magic == constants::MAGIC_NUMBER);
        assert(sp->s_log_block_size == 0);  // 0 for ext2
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
void test_fs_ext2() {
    /**
     * extra information
     */
    {
        printf("sizeof SUPER%10c     :=%ld bytes\n", 0, sizeof(FS::SUPER));
        printf("sizeof INODE%10c     :=%ld bytes\n", 0, sizeof(FS::INODE));
        printf("sizeof DIR_ENTRY%10c :=%ld bytes\n", 0, sizeof(FS::DIR_ENTRY));
        printf("\n");
    }
    Test::Header("confirms EXT2 filesystem");
    bool result = Test::Body(*_test_fs_ext2);
    Test::Footer(result);
}
