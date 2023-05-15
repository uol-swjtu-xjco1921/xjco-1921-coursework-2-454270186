#ifndef _FILE_IO
#define _FILE_IO
#include "models.h"
#include "vector.h"
#include "hashtable.h"
#include "utility.h"

#define MAX_LINE_LENGTH 2000

extern edge_vector edges;

typedef enum {
    BOUND,
    LINK,
    NODE,
    ELSE,
} DATA_TYPE;

/*
    FILE Read
*/
int read_file(const char* filename, Node* node_table[], Node* adj_table[], Node* spd_adj_table[], edge_vector* edges, node_vector* nodes, Bound* bd);
int read_bound(const char* buf, Bound* bd);
int read_edge(char* buf, edge_vector* edges, Node* adj_table[], Node* spd_table[]);
int read_node(const char* buf, node_vector* nodes, Node* node_table[]);
DATA_TYPE check_type(const char* buf);

/*
    FILE Write
*/
int add_link(const char* filename);
int mod_link_attri(const char* filename, int64_t link_id, char* attri_name, double attri_val);

#endif