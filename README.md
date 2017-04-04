# File Recovery in ext2 file system,

The aim of project was to develop the tool in C.

The architecure of ext2 system is as following

Whole disk is divided into block groups, this division is based on the size of the disk, for usb disk of 2GB, 16 block gruop were created by system as per ext2 architecture. Each block gruop containns following in same order 

1. Superblock (Not all block gruop conatains superblock)
  Superblock has all information reated to the whole file system such as no of block gropus, number of inodes, numbe of blocks,       number of free inodes, number of free blocks, creation time of file system, location of first inode etc.
2. Group Descriptor
  Group descriptor contains information about the particular block group, such as first free block, number of free inodes in the     gruop, number of free blocks in group, etc.
3. Inode Bitmap:
  Each bit represents corresponding inode free or allocated as per value 0 or 1.
4. Block Bitmap
  Same as inode bitmap, but for data blocks
5. Inode table
  Inode table contains the inodes corresponding to the files in the disk. Each inode contains many infomation such as its creation time, its mode, filesize, and datablocks etc.
6. Data Blocks
  These are the data block which stores the data based on the file content.
  

We used above knowledge to reach the location of the folder, which has deleted file to recover starting with root inode which is 2. We then read the corresponding data block of the directory, which in turn contains directory entries for the files residing in the directory, based on the directory entries we can find out the deleted files names and the inode numbers. And then try to relocate the data blocks associated based on the magic numbers of particular file format.
