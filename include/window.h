#ifndef _WINDOW
#define _WINDOW
#include <gtk/gtk.h>
#include "models.h"
#include "vector.h"
#include "hashtable.h"
#include "drawer.h"

extern Bound bd;
extern edge_vector edges;
extern node_vector nodes;
extern node_vector path;
extern Node* node_table[TABLE_SIZE];
extern Node* adj_table[TABLE_SIZE];


/* init */
void window_init(Bound* bd, edge_vector* edges, 
                 node_vector* nodes, node_vector* path,
                 Node* node_table[],
                 Node* adj_table[]);

/* run */
void window_start(const char* filename);
void window_clear();

/* Renderer */
void main_window_renderer();
void map_window_renderer();
void get_input_renderer();
void shortest_window_renderer();

/* Callback func */
void draw_the_map();
void map_back_to_main();
void hide_input();
void deal_input();

#endif