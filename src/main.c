#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat_parser.h"
#include "fuse_fat.h"
#include "rest_api.h"
#include "db.h"
#include "thread_pool.h"

void print_usage(const char* prog_name) {
    printf("Usage: %s <command> [options]\n", prog_name);
    printf("Commands:\n");
    printf("  analyze <FAT32_image>   - Analyze and list FAT metadata.\n");
    printf("  mount <FAT32_image> <mountpoint>   - Mount FAT image via FUSE.\n");
    printf("  api <FAT32_image> <port>  - Run REST API server on given port.\n");
    printf("  db <FAT32_image> <db_file>  - Insert FAT metadata into SQLite DB.\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    const char* command = argv[1];
    if (strcmp(command, "analyze") == 0) {
        const char* image_path = argv[2];
        void* mapped_addr = NULL;
        size_t mapped_size = 0;
        FAT32_BootSector bs;
        if (read_boot_sector_mmap(image_path, &bs, &mapped_addr, &mapped_size) != 0)
            return EXIT_FAILURE;
        printf("FAT32 Volume Label: %.11s\n", bs.BS_VolLab);
        printf("Bytes Per Sector: %u\n", bs.BPB_BytsPerSec);
        printf("Sectors Per Cluster: %u\n", bs.BPB_SecPerClus);
        list_directory_mmap(mapped_addr, &bs, bs.BPB_RootClus);
        munmap(mapped_addr, mapped_size);
    }
    else if (strcmp(command, "mount") == 0) {
        if (argc < 4) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        const char* image_path = argv[2];
        const char* mountpoint = argv[3];
        return run_fuse_fat(image_path, mountpoint);
    }
    else if (strcmp(command, "api") == 0) {
        if (argc < 4) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        const char* image_path = argv[2];
        int port = atoi(argv[3]);
        return start_rest_api_server(port, image_path);
    }
    else if (strcmp(command, "db") == 0) {
        if (argc < 4) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        const char* image_path = argv[2];
        const char* db_file = argv[3];
        void* mapped_addr = NULL;
        size_t mapped_size = 0;
        FAT32_BootSector bs;
        if (read_boot_sector_mmap(image_path, &bs, &mapped_addr, &mapped_size) != 0)
            return EXIT_FAILURE;
        db_init(db_file);
        db_insert_metadata(db_file, &bs);
        munmap(mapped_addr, mapped_size);
        printf("Metadata inserted into %s\n", db_file);
    }
    else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
