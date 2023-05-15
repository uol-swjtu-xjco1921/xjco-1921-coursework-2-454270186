#ifndef _DRAWER
#define _DRAWER
#include <plplot/plplot.h>
#include "models.h"
#include "vector.h"

/*
    Pre-check
*/
int is_edges_empty(edge_vector* edges);

/*
    Drawer func
*/
int draw_edges(Bound* bd, edge_vector* edges, Node* table[]);
int draw_path(Bound* bd, node_vector* path, edge_vector* edges, Node* table[], int mode);
int draw_loc_path(Bound* bd, node_vector* path_1, node_vector* path_2, edge_vector* edges, Node* table[]);
#endif