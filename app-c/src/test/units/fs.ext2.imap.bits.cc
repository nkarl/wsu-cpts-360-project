#include "../../hdr-locked/constants.hpp"
#include "../../hdr-locked/my-fs.hpp"
#include "../../hdr-locked/my-types.hpp"

#include "all-tests.hpp"

#include <cassert>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2_imap_bits() {
    /**
     * SET UP
     */
    const u8 value_1 = 80, value_2 = 112;
    u8       bit_index = 5;

    /**
     * TEST BODY
     */
    {
        /**
         * test setting a bit in a byte.
         *
         *                    76543210
         * value  =  80d -> 0x01010000
         * bit_index     ->     5
         * result = 112d -> 0x01110000
         */
        auto bit_set_result = FS::Utils::set_bit(value_1, bit_index) == value_2;

        /**
         * test clearing a bit from a byte.
         *
         *                    76543210
         * value  = 112d -> 0x01110000
         * bit_index     ->     5
         * result =  80d -> 0x01010000
         */
        auto bit_clear_result = FS::Utils::clear_bit(value_2, bit_index) == value_1;
        assert(bit_set_result && bit_clear_result);
    }

    /**
     * TEAR DOWN
     */
    return true;
}

/**
 * test executor.
 */
void test_fs_ext2_imap_bits() {
    Test::Header("sets and clears a bit in a byte (8-bit map)");
    bool result = Test::Body(*_test_fs_ext2_imap_bits);
    Test::Footer(result);
}
