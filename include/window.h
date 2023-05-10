#ifndef _WINDOW
#define _WINDOW
#include <gtk/gtk.h>
#include "models.h"
#include "vector.h"
#include "hashtable.h"
#include "drawer.h"

/* init */
void window_init(Bound* bd, edge_vector* edges, 
                 node_vector* nodes, node_vector* path,
                 Node* node_table[],
                 Node* adj_table[]);

/* run */
void window_start();
void window_clear();

/* Renderer */
void main_window_renderer();
void map_window_renderer();

/* Callback func */
void draw_the_map();

#endif