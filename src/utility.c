#include <stdio.h>
#include <string.h>
#include <ctype.h>
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