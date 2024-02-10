#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "my-types.hh"

namespace constants {
    constexpr u32 BASE_BLOCK_SIZE = 1024;
    constexpr u32 MAGIC_NUMBER    = 0xef53;
    constexpr u32 SUPER_BLOCK     = 1;
    constexpr u32 GD_BLOCK        = 2;
    constexpr u32 ROOT_INODE      = 2;

    constexpr u32 DIR_MODE  = 0x41ed;
    constexpr u32 FILE_MODE = 0x81ae;

    /*
     * contains other constants and enum.
     */
    // constexpr u32 SUPER_USER = 0;

}  // namespace constants

#endif
