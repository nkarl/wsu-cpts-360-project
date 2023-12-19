#include "hdr-locked/my-types.hpp"

#include "hdr-locked/my-fs.hpp"
#include "test-runner.cc"

i32 main(i32 argc, i8 *argv[]) {
    test_runner();

    for (i8 i = 0; i < argc; ++i) {
        printf("%d %s\n", i, argv[i]);
    }

    i8 *diskname;
    if (argc >= 1) {
        diskname = argv[1];
    }
    else {
        exit(1);
    }

    FS::EXT2 vdisk(diskname);
    FS::Read::EXT2::block_super(&vdisk);
    FS::Show::EXT2::block_super(&vdisk);
    FS::Read::EXT2::block_group_desc(&vdisk);
    FS::Show::EXT2::block_group_desc(&vdisk);

    return 0;
}
