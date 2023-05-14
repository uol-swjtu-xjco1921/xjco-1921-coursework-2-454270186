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
extern Node* node_table[];
extern Node* adj_table[];
extern Node* spd_adj_table[];

/* run */
void window_start();
void window_clear();
void window_quit();

/* Renderer */
void main_window_renderer();
void map_window_renderer();
void get_input_renderer();
void get_input_fast_renderer();
void get_input_loc_renderer();
void get_loc_renderer();
void shortest_window_renderer();
void fastest_window_renderer();
void constraint_window_renderer();
void edit_window_renderer();
void loc_shortest_window_renderer();
void error_window_renderer();

/* Callback func */
void draw_the_map();
void map_back_to_main();
void hide_input();
void deal_input();
void deal_fast_input();
void deal_s_e_loc_input();
void deal_loc_input();
void deal_edit_input();
void hide_error();
void hide_cons();
void hide_loc_input();
void hide_edit();

#endif