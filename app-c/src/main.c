#include "hdr/add.h"
#include "hdr/sub.h"
#include <stdio.h>

int main() {
    printf("%s", "hello");
    int a = add(2, 5);
    printf("add 2 5 == %d\n", a);
    int b = sub(5, 2);
    printf("sub 5 2 == %d\n", b);
    return 0;
}
