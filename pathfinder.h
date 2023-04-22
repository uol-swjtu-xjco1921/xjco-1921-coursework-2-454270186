#ifndef _PATH_FINDER
#define _PATH_FINDER
#include "models.h"
#include "hashtable.h"

/*
    External interface
*/
int find_path();

/*
    DfS: Find all paths between two points
*/
void dfs(int64_t current, int64_t end, int64_t* shortest_len, int64_t path[], int path_index, Node* adj_table[], Node* node_table[]);

#endif