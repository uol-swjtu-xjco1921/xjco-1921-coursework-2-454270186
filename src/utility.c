#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include "utility.h"

/// @brief Trim spaces at the beginning and end of a string
/// @param str the str to trim
/// @return Trimmed string
char* trim_space(char* str) {
    size_t len = strlen(str);

    while (len > 0 && isspace(str[len-1])) {
        str[--len] = '\0';
    }

    char* str_start = str;
    while (*str_start && isspace(*str_start)) {
        ++str_start;
        --len;
    }

    memmove(str, str_start, len + 1);
    return str;
}


int64_t get_link_id(const char* str) {
    const char* start = strstr(str, "id=");

    if (start != NULL) {
        start += 3; // skip "id="
        const char* end = strchr(start, ' ');

        if (end != NULL) {
            size_t length = end - start;

            char* linkIdStr = (char*)malloc(length + 1);
            strncpy(linkIdStr, start, length);
            linkIdStr[length] = '\0';

            int64_t linkId = strtoll(linkIdStr, NULL, 10);

            free(linkIdStr);

            return linkId;
        }
    }

    return 0;
}