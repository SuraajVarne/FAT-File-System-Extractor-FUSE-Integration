#ifndef FUSE_FAT_H
#define FUSE_FAT_H

// Initialize and run the FUSE file system to mount the FAT image.
// 'image_path' is the path to the FAT32 image and 'mountpoint' is where it will be mounted.
int run_fuse_fat(const char* image_path, const char* mountpoint);

#endif // FUSE_FAT_H
#pragma once
