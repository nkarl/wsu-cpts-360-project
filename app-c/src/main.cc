#include "utils.cc"

int main(int argc, char *argv[]) {
    test_boot_compile_link();

    for (int i = 0; i < argc; ++i) {
        printf("%d %s\n", i, argv[i]);
    }

    char *diskname;
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
