#ifndef _DRAWER
#define _DRAWER
#include <plplot/plplot.h>
#include "models.h"
#include "vector.h"

int draw_edges(Bound* bd, edge_vector* edges, Node* table[]);
int input_edges(edge_vector* edges, Node* table[]);

#endif