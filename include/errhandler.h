#ifndef _ERR_HANDLER
#define _ERR_HANDLER
#include "vector.h"

#define USAGE 0
#define WRONG_PREFIX -1
#define WRONG_SUFFIX -2
#define BAD_ARG_COUNT -3
#define BAD_FILE_NAME -4

void check_cmd_arg(int argc, char** argv);
int check_line_fmt(const char* line, const char* prefix, const char* suffix);
int check_edge_existence(edge_vector* edges, int64_t link_id);
int check_node_existence(Node* node_table[], int64_t node_id);
int check_POI_existence(edge_vector* edges, int POI);

#endif