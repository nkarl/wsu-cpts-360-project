#include "../hdr/utils.hh"

void print(i8 const *const s, u32 x) {
    printf("%-30s = %8d\n", s, x);
}

void print(i8 const *const s, i8 const *const x) {
    printf("%-30s = %8s\n", s, x);
}

void print(i8 const *const s, u32 x, i8 const *const) {
    printf("%-30s = %8x\n", s, x);
}
