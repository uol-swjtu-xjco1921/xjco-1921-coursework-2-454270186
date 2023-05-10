#include <gtk/gtk.h>
#include "window.h"
#include "models.h"
#include "vector.h"
#include "hashtable.h"
#include "drawer.h"
#include "logger.h"
#include "log.h"

Bound* win_bd;
edge_vector* win_edges;
node_vector* win_nodes;
node_vector* win_path;
Node* win_node_table[TABLE_SIZE];
Node* win_adj_table[TABLE_SIZE];

GtkBuilder* builder;
GObject* window;
GObject* map_window;
GObject* button;
GError *error = NULL;

void window_init(Bound* bd, edge_vector* edges, 
                 node_vector* nodes, node_vector* path,
                 Node* node_table[],
                 Node* adj_table[]) 
{
    win_bd = bd;
    win_edges = edges;
    win_nodes = nodes;
    win_path = path;
    for (int i = 0; i < TABLE_SIZE; i++) {
        win_node_table[i] = node_table[i];
        win_adj_table[i] = adj_table[i];
    }
}

// callback func for quit
void window_quit() {
    window_clear();
    gtk_main_quit();
}

void window_start() {
    gtk_init(NULL, NULL);
    main_window_renderer();
    gtk_main();
}

void window_clear() {
    e_vector_free(win_edges);
    n_vector_free(win_nodes);
    n_vector_free(win_path);
    free_table(win_node_table);
    free_adj_table(win_adj_table);
}

// callback
void draw_the_map() {
    int ret = draw_edges(win_bd, win_edges, win_node_table);
    if (ret != 0) {
        ERROR("Unexpect internal error");
        return;
    }

    gtk_widget_hide(GTK_WIDGET(window));
    map_window_renderer();
}

// callback
void back_to() {
    gtk_widget_hide(GTK_WIDGET(map_window));
    gtk_widget_show(GTK_WIDGET(window));
}

void main_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/index.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "button1");
    g_signal_connect(button, "clicked", G_CALLBACK(draw_the_map), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
}

void map_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/image.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    map_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (map_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(back_to), NULL);

    gtk_widget_show_all(GTK_WIDGET(map_window));
}