#include <stdio.h>
#include "hdr/add.h"

int main()
{
    printf("%s", "hello");
    int a = add(2, 5);
    printf("%d", a);
    return 0;
}
