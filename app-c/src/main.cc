#include "hdr/my_types.hpp"

#include "utils.cc"
#include "tests/test_operators.cc"

i32 main(i32 argc, i8 *argv[]) {
    test_boot_compile_link();

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

    VDisk vdisk;
    vdisk.readSUPER(diskname);

    printf("i32=%ld\ti64=%ld bytes\n", sizeof(i32), sizeof(i64));
    printf("u32=%ld\tu64=%ld bytes\n", sizeof(u32), sizeof(u64));

    return 0;
}
