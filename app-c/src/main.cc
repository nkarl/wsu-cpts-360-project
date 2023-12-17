#include "hdr/my-types.hpp"

#include "temp-workspace.cc"
#include "test-runner.cc"

i32 main(i32 argc, i8 *argv[]) {
    test_runner();

    for (i8 i = 0; i < argc; ++i) {
        printf("%d %s\n", i, argv[i]);
    }

    printf("blksize=%ld\n", sizeof(u32) * constants::BASE_BLOCK_SIZE);

    i8 *diskname;
    if (argc >= 1) {
        diskname = argv[1];
    }
    else {
        exit(1);
    }

    VDisk vdisk;
    vdisk.readSUPER(diskname);

    return 0;
}
