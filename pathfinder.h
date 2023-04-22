#ifndef _PATH_FINDER
#define _PATH_FINDER
#include <stdint.h>
#include "models.h"
#include "hashtable.h"
#include "vector.h"
#include "heap.h"


/*
    External interface
*/
void print_path(Node* pre_table[], int64_t start_node_id, int64_t end_node_id);
int find_path();

/*
    Dijkstra
*/
void dijkstra(node_vector* nodes, Node* adj_table[], int64_t start_node_id, int64_t end_node_id);

#endif