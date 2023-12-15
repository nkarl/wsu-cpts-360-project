#include "hdr/add.hpp"
#include "hdr/sub.hpp"
#include <stdio.h>

void boot_test() {
    printf("%s", "\nTest -- Compiling and Linking Objects: ");
    int a = add(2, 5);
    printf("add 2 5 == %d, ", a);
    int b = sub(5, 2);
    printf("sub 5 2 == %d\n\n", b);
}


int print(char *s, unsigned int x)
{
printf("%-30s = %8d\n", s, x);
}
