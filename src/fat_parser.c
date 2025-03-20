#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fat_parser.h"
#include "utils.h"

int read_boot_sector_mmap(const char* image_path, FAT32_BootSector* bs, void** mapped_addr, size_t* mapped_size) {
    int fd = open(image_path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening image file");
        return -1;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("Error getting file size");
        close(fd);
        return -1;
    }
    *mapped_size = st.st_size;
    *mapped_addr = mmap(NULL, *mapped_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (*mapped_addr == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return -1;
    }
    close(fd);
    memcpy(bs, *mapped_addr, sizeof(FAT32_BootSector));
    return 0;
}

uint32_t cluster_to_offset(FAT32_BootSector* bs, uint32_t cluster) {
    uint32_t firstDataSector = bs->BPB_RsvdSecCnt + (bs->BPB_NumFATs * bs->BPB_FATSz32);
    uint32_t sector = firstDataSector + ((cluster - 2) * bs->BPB_SecPerClus);
    return sector * bs->BPB_BytsPerSec;
}

int list_directory_mmap(void* mapped_addr, FAT32_BootSector* bs, uint32_t cluster) {
    uint32_t offset = cluster_to_offset(bs, cluster);
    FAT32_DirEntry* entries = (FAT32_DirEntry*)((uint8_t*)mapped_addr + offset);
    uint32_t entries_per_cluster = (bs->BPB_SecPerClus * bs->BPB_BytsPerSec) / sizeof(FAT32_DirEntry);
    printf("Directory listing for cluster %u:\n", cluster);
    for (uint32_t i = 0; i < entries_per_cluster; i++) {
        if (entries[i].DIR_Name[0] == 0x00) continue;
        if ((unsigned char)entries[i].DIR_Name[0] == 0xE5) continue;
        char name[32];
        format_filename(entries[i].DIR_Name, name);
        if (entries[i].DIR_Attr & 0x10) {
            printf("[DIR]  %s\n", name);
        }
        else {
            printf("[FILE] %s    Size: %u bytes\n", name, entries[i].DIR_FileSize);
        }
    }
    return 0;
}
