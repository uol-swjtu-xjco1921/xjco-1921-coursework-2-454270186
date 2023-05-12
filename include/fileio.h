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
} DATA_TYPE;

typedef enum {
    LENGTH,
    SPEED,
} EDGE_READ_MODE;

/*
    FILE Read
*/
int read_file(const char* filename, Node* node_table[], Node* adj_table[], Node* spd_adj_table[], edge_vector* edges, node_vector* nodes, Bound* bd);
int read_bound(const char* buf, Bound* bd);
int read_edge(const char* buf, edge_vector* edges, Node* adj_table[], EDGE_READ_MODE mode);
int read_node(const char* buf, node_vector* nodes, Node* node_table[]);
DATA_TYPE check_type(const char* buf);

/*
    FILE Write
*/
int add_link(const char* filename);
int add_speed_lim(const char* filename);
int modify_speed_lim(const char* filename);

#endif