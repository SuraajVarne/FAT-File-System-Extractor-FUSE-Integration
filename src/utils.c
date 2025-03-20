#include <stdio.h>
#include <string.h>
#include "utils.h"

void format_filename(const char* raw, char* formatted) {
    char name[9] = { 0 };
    char ext[4] = { 0 };
    memcpy(name, raw, 8);
    memcpy(ext, raw + 8, 3);
    for (int i = 7; i >= 0; i--) {
        if (name[i] == ' ')
            name[i] = '\0';
        else
            break;
    }
    for (int i = 2; i >= 0; i--) {
        if (ext[i] == ' ')
            ext[i] = '\0';
        else
            break;
    }
    if (ext[0] != '\0')
        sprintf(formatted, "%s.%s", name, ext);
    else
        sprintf(formatted, "%s", name);
}
