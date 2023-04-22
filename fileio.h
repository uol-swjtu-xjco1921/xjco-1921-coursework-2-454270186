#ifndef _FILE_IO
#define _FILE_IO
#define MAX_LINE_LENGTH 200
#include "models.h"
#include "vector.h"
#include "hashtable.h"

int read_file(const char* filename, Node* node_table[], Node* adj_table[], edge_vector* edges, Bound* bd);
int read_bound(FILE* map_file, Bound* bd);
int read_edge(FILE* map_file, edge_vector* edges, Node* adj_table[]);
int read_node(FILE* map_file, Node* node_table[]);

#endif