#include "../hdr-locked/constants.hpp"
#include "../hdr-locked/my-fs.hpp"
#include "../hdr-locked/my-types.hpp"

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
        u8 value     = 80;
        u8 bit_index = 5;
        u8 expect    = 112;

        u32 result = FS::Show::EXT2::set_bit(value, bit_index);
        assert(result == expect);

        /**
         * test clearing a bit from a byte.
         *
         *                    76543210
         * value  = 112d -> 0x01110000
         * bit_index     ->     5
         * result =  80d -> 0x01010000
         */
        value     = 112;
        bit_index = 5;
        expect    = 80;

        result = FS::Show::EXT2::clear_bit(value, bit_index);
        assert(result == expect);
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
