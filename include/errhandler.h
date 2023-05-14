#ifndef _ERR_HANDLER
#define _ERR_HANDLER
#include "vector.h"

#define WRONG_PREFIX -1
#define WRONG_SUFFIX -2

void check_cmd_arg(int argc, char** argv);
int check_line_fmt(const char* line, const char* prefix, const char* suffix);
int check_edge_existence(edge_vector* edges, int64_t link_id);

#endif