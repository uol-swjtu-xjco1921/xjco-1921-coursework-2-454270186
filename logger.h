#ifndef _LOGGER
#define _LOGGER

#define ERROR(msg) do { \
    printf("\033[0;31m"); \
    printf("%s", msg); \
    printf("\033[0m"); \
} while (0)

#define INFO(msg) do { \
    printf("\033[0;32m"); \
    printf("%s", msg); \
    printf("\033[0m"); \
} while (0)

#endif