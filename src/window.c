#include <gtk/gtk.h>
#include "window.h"
#include "models.h"
#include "vector.h"
#include "hashtable.h"
#include "drawer.h"
#include "logger.h"
#include "log.h"
#include "pathfinder.h"
#include "fileio.h"

// Bound bd;
// edge_vector edges;
// node_vector nodes;
// node_vector path;
// Node* node_table[TABLE_SIZE] = {NULL};
// Node* adj_table[TABLE_SIZE] = {NULL};

GtkBuilder* builder;
GObject* window;
GObject* map_window;
GObject* shortest_window;
GObject* input_window;
GObject* button;
GObject* entry;
GError* error = NULL;

int64_t start, end;

// void window_init(Bound* bd, edge_vector* edges, 
//                  node_vector* nodes, node_vector* path,
//                  Node* node_table[],
//                  Node* adj_table[]) 
// {
//     bd = bd;
//     win_edges = edges;
//     win_nodes = nodes;
//     // win_path = path;
//     for (int i = 0; i < TABLE_SIZE; i++) {
//         win_node_table[i] = node_table[i];
//         win_adj_table[i] = adj_table[i];
//     }
//     printf("window has initialized\n");
// }

// callback func for quit
void window_quit() {
    window_clear();
    gtk_main_quit();
}

void window_start(const char* filename) {
    gtk_init(NULL, NULL);
    main_window_renderer();
    gtk_main();
}

void window_clear() {
    e_vector_free(&edges);
    n_vector_free(&nodes);
    n_vector_free(&path);
    free_table(node_table);
    free_adj_table(adj_table);
}

/*
    Button click callback func
*/
void draw_the_map() {
    if (access("map.png", F_OK) != 0) {
        int ret = draw_edges(&bd, &edges, node_table);
        if (ret != 0) {
            ERROR("Unexpect internal error");
            return;
        } 
    }
    
    gtk_widget_hide(GTK_WIDGET(window));
    map_window_renderer();
}

void draw_shortest() {
    if (access("shorest.png", F_OK) != 0) {
        int ret = draw_shortest_path(&bd, &path, &edges, node_table);
        if (ret != 0) {
            ERROR("Unexpect internal error");
            return;
        }
    }

    gtk_widget_hide(GTK_WIDGET(input_window));
    shortest_window_renderer();
}

void get_user_input() {
    get_input_renderer();
}

/// @brief input.xml Next button clicked
void deal_input() {
    const gchar* input = gtk_entry_get_text(GTK_ENTRY(entry));

    printf("%s\n", input);
    int ret = sscanf(input, "%ld %ld", &start, &end);
    if (ret != 2) {
        // TODO: 展示错误页面
    }
}

void map_back_to_main() {
    gtk_widget_hide(GTK_WIDGET(map_window));
    gtk_widget_show(GTK_WIDGET(window));
}

void hide_input() {
    gtk_widget_hide(GTK_WIDGET(input_window));
}

/*
    Renderer
*/
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

    button = gtk_builder_get_object(builder, "button2");
    g_signal_connect(button, "clicked", G_CALLBACK(get_user_input), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
}

void map_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/map.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    map_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (map_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(map_back_to_main), map_window);

    gtk_widget_show_all(GTK_WIDGET(map_window));
}

void shortest_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/shortest.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    shortest_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (shortest_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(map_back_to_main), shortest_window);

    gtk_widget_show_all(GTK_WIDGET(shortest_window));
}

void get_input_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/input.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    input_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (input_window, "destroy", G_CALLBACK (window_quit), NULL);

    // back button
    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_input), NULL);

    // next button
    entry = gtk_builder_get_object(builder, "entry_input");
    button = gtk_builder_get_object(builder, "next_button");
    g_signal_connect(button, "clicked", G_CALLBACK(deal_input), NULL);

    gtk_widget_show_all(GTK_WIDGET(input_window));
}