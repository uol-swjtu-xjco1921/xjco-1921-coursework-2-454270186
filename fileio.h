#ifndef _FILE_IO
#define _FILE_IO
#include "models.h"
#include "vector.h"
#include "hashtable.h"

#define MAX_LINE_LENGTH 200

typedef enum {
    BOUND,
    LINK,
    NODE,
    SPEED,
} DATA_TYPE;

int read_file(const char* filename, Node* node_table[], Node* adj_table[], edge_vector* edges, node_vector* nodes, Bound* bd);
DATA_TYPE check_type(const char* buf);
int read_bound(const char* buf, Bound* bd);
int read_edge(const char* buf, edge_vector* edges, Node* adj_table[]);
int read_node(const char* buf, node_vector* nodes, Node* node_table[]);

#endif