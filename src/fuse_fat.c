#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "fuse_fat.h"
#include "fat_parser.h"
#include "utils.h"

// Global variables for simplicity (could be encapsulated in a context structure).
static void* g_mapped_addr = NULL;
static size_t g_mapped_size = 0;
static FAT32_BootSector g_bs;

static int fat_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi) {
    (void)fi;
    memset(stbuf, 0, sizeof(struct stat));
    // For root directory
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0555;
        stbuf->st_nlink = 2;
        return 0;
    }
    // For demo purposes, treat every file as a regular file.
    stbuf->st_mode = S_IFREG | 0444;
    stbuf->st_nlink = 1;
    stbuf->st_size = 1024;  // Dummy size.
    return 0;
}

static int fat_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags) {
    (void)offset;
    (void)fi;
    (void)flags;
    // Only support root directory for this demo.
    if (strcmp(path, "/") != 0)
        return -ENOENT;
    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);
    // List directory from root cluster.
    // In a real implementation, you'd iterate over the FAT directory entries.
    char name[32];
    // Dummy entry:
    strcpy(name, "FILE.TXT");
    filler(buf, name, NULL, 0, 0);
    return 0;
}

static const struct fuse_operations fat_oper = {
    .getattr = fat_getattr,
    .readdir = fat_readdir,
};

int run_fuse_fat(const char* image_path, const char* mountpoint) {
    // Memory-map the image and read boot sector.
    if (read_boot_sector_mmap(image_path, &g_bs, &g_mapped_addr, &g_mapped_size) != 0) {
        return -1;
    }
    // Prepare FUSE arguments.
    char* fuse_argv[] = { (char*)"fuse_fat", (char*)mountpoint, "-f", NULL };
    int fuse_argc = 3;
    int ret = fuse_main(fuse_argc, fuse_argv, &fat_oper, NULL);
    munmap(g_mapped_addr, g_mapped_size);
    return ret;
}
