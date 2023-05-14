#ifndef _INIT
#define _INIT
#include <string.h>
#include "models.h"
#include "vector.h"
#include "hashtable.h"
#include "fileio.h"
#include "log.h"

extern Bound bd;
extern edge_vector edges;
extern node_vector nodes;
extern node_vector path;
extern Node* node_table[];
extern Node* adj_table[];
extern Node* spd_adj_table[];
extern char* filename;

void init();

void clear();

void refresh();

#endif