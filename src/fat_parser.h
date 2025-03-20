#ifndef FAT_PARSER_H
#define FAT_PARSER_H

#include <stdint.h>
#include <stddef.h>
#include "../include/fat_defs.h"

// Memory-map the FAT32 image and read its boot sector.
int read_boot_sector_mmap(const char* image_path, FAT32_BootSector* bs, void** mapped_addr, size_t* mapped_size);

// Convert a FAT32 cluster number to its file offset.
uint32_t cluster_to_offset(FAT32_BootSector* bs, uint32_t cluster);

// List directory entries for a given cluster using memory-mapped image.
int list_directory_mmap(void* mapped_addr, FAT32_BootSector* bs, uint32_t cluster);

#endif // FAT_PARSER_H
#pragma once
