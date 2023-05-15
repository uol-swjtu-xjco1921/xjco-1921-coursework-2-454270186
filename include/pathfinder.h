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
node_vector print_path(Node* pre_table[], Node* node_table[], int64_t start_node_id, int64_t end_node_id);

/*
    Dijkstra
*/
node_vector dijkstra(node_vector* nodes, Node* adj_table[], Node* node_table[], int64_t start_node_id, int64_t end_node_id, double* final_dis);
node_vector dijk_poi(node_vector* nodes, Node* adj_table[], Node* node_table[], edge_vector* edges, int target_poi, int64_t start, int64_t end);

#endif