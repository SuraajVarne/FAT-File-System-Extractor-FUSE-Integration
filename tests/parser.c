#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int fat32_parser() {
    printf("FAT32 Parser running in WebAssembly\n");
    return 0;
}
