#ifndef _MODELS
#define _MODELS
#include <stdint.h>

typedef struct _adj_list{
    struct _node* neighbor_node;
    double length;
    struct _adj_list* next;
} Adj_list;

typedef struct _node{
    int64_t id;
    double lat;
    double lon;
    struct _node* next;
    Adj_list* adj_list;
    int is_visited;
    double dis;
    int64_t next_id; // used for path backtracking
} Node;

typedef struct {
    int64_t id;
    int64_t from;
    int64_t to;
    int32_t way;
    double length;
    double veg;
    double arch;
    double land;
    int32_t POI;
} Edge;

typedef struct {
    double min_lat;
    double max_lat;
    double min_lon;
    double max_lon;
} Bound;

#endif