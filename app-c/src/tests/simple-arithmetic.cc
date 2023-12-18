#include "../hdr-locked/my-types.hpp"

#include <cassert>

#include "../hdr/add.hpp"
#include "../hdr/sub.hpp"

void test_boot_compile_link() {
    assert((void("\nTest Add: "), add(2, 5) == 7));
    assert((void("\nTest Sub: "), sub(5, 2) == 3));
}
