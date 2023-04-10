#ifndef _FILE_IO
#define _FILE_IO
#define MAX_LINE_LENGTH 200
#include "models.h"

int read_file(const char* filename, node* nodes, edge* edges, bound* bound_line);

#endif