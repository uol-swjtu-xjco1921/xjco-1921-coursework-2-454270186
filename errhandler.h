#ifndef _ERR_HANDLER
#define _ERR_HANDLER

#define WRONG_PREFIX -1
#define WRONG_SUFFIX -2

void check_cmd_arg(int argc);
int check_line_fmt(const char* line, const char* prefix, const char* suffix);

#endif