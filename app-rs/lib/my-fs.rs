use super::*;

type SUPER = ext2_super_block;
type GD = ext2_group_desc;
type INODE = ext2_inode;
type DIR_ENTRY = ext2_dir_entry_2;

pub struct EXT2 {
    pub device_name: std::String,
    pub fd: i32,
    blksize: u32,
    first_ino: u32,
}
