#include <stdio.h>
#include <string.h>
#include "errhandler.h"

int check_line_fmt(const char* line, const char* prefix, const char* suffix) {
    // check prefix
    int ret = strncmp(line, prefix, strlen(prefix));
    if (ret != 0) {
        printf("ERROR: Can not read line (wrong prefix)\n");
        return -1;
    }

    // check suffix
    char* str = strstr(line, suffix);
    if (str == NULL) {
        printf("ERROR: Can not read line (wrong suffix)\n");
        return -1;
    }

    return 0;
}