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
#include "utility.h"

GtkBuilder* builder;
GObject* window;
GObject* map_window;
GObject* shortest_window;
GObject* fastest_window;
GObject* loc_shortest_window;
GObject* input_window;
GObject* loc_input_window;
GObject* cons_window;
GObject* error_window;
GObject* button;
GObject* entry;
GError* error = NULL;

int64_t start = 0, end = 0;

void window_start() {
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
    free_adj_table(spd_adj_table);
}

void window_quit() {
    window_clear();
    gtk_main_quit();
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
    int ret = draw_path(&bd, &path, &edges, node_table, 0);
    if (ret != 0) {
        ERROR("Unexpect internal error");
        return;
    }

    gtk_widget_hide(GTK_WIDGET(input_window));
    shortest_window_renderer();
}

void draw_fastest() {
    int ret = draw_path(&bd, &path, &edges, node_table, 1);
    if (ret != 0) {
        ERROR("Unexpect internal error");
        return;
    }

    gtk_widget_hide(GTK_WIDGET(input_window));
    fastest_window_renderer();
}

void get_user_input() {
    get_input_renderer();
}

void get_user_input_fast() {
    get_input_fast_renderer();
}

void get_user_loc() {
    get_loc_renderer();
}

/// @brief input.xml Next button clicked
void deal_input() {
    const gchar* input = gtk_entry_get_text(GTK_ENTRY(entry));

    // int64_t start = 0, end = 0;
    int ret = sscanf(input, "%ld %ld", &start, &end);
    if (ret != 2) {
        // TODO: 展示错误页面
        error_window_renderer();
        return;
    }

    // shortest route
    path = dijkstra(&nodes, adj_table, node_table, start, end);
    if (path.size == 0) {
        ERROR("Unexpect internal error while find shortest route");
    }

    draw_shortest();
}

void deal_fast_input() {
    const gchar* input = gtk_entry_get_text(GTK_ENTRY(entry));

    // int64_t start = 0, end = 0;
    int ret = sscanf(input, "%ld %ld", &start, &end);
    if (ret != 2) {
        // TODO: 展示错误页面
        error_window_renderer();
        return;
    }

    // fastest route
    path = dijkstra(&nodes, spd_adj_table, node_table, start, end);
    if (path.size == 0) {
        ERROR("Unexpect internal error while find shortest route");
    }

    draw_fastest();
}

void deal_s_e_loc_input() {
    const gchar* input = gtk_entry_get_text(GTK_ENTRY(entry));
    if (start == 0 && end == 0) {
       int ret = sscanf(input, "%ld %ld", &start, &end);
        if (ret != 2) {
            // TODO: 展示错误页面
            error_window_renderer();
            return;
        } 
    }
    
    get_loc_renderer();
}

void deal_loc_input() {
    const gchar* temp = gtk_entry_get_text(GTK_ENTRY(entry));
    gchar* input = trim_space((char*)temp);

    // check input is ID or location
    int ret = -1;
    int64_t node_id = 0;
    char* space = strchr(input, ' ');
    if (space == NULL) {
        // ID
        ret = sscanf(input, "%ld", &node_id);
        if (ret != 1) {
            // ERROR
            return;
        }
    } else {
        // location
        double lat, lon;
        ret = sscanf(input, "%lf %lf", &lat, &lon);
        if (ret != 2) {
            // ERROR
            return;
        }

        node_id = search_by_loc(node_table, lat, lon);
        if (node_id == 0) {
            // ERROR
            return;
        }
    }

    node_vector path_2;
    n_vector_init(&path_2, 10);
    path = dijkstra(&nodes, adj_table, node_table, start, node_id);
    path_2 = dijkstra(&nodes, adj_table, node_table, node_id, end);
    if (path.size == 0 || path_2.size == 0) {
        ERROR("Unexpect internal error while find shortest route");
    }

    draw_loc_path(&bd, &path, &path_2, &edges, node_table);
    hide_cons();
    hide_loc_input();
    hide_input();
    loc_shortest_window_renderer();
}

void map_back_to_main() {
    gtk_widget_hide(GTK_WIDGET(map_window));
    gtk_widget_show(GTK_WIDGET(window));
}

void shortest_back_to_main() {
    gtk_widget_hide(GTK_WIDGET(shortest_window));
    gtk_widget_show(GTK_WIDGET(window));
}

void fastest_back_to_main() {
    gtk_widget_hide(GTK_WIDGET(fastest_window));
    gtk_widget_show(GTK_WIDGET(window));
}

void location_back_to_main() {
    gtk_widget_hide(GTK_WIDGET(loc_shortest_window));
    gtk_widget_show(GTK_WIDGET(window));
}

void hide_input() {
    gtk_widget_hide(GTK_WIDGET(input_window));
}

void hide_loc_input() {
    gtk_widget_hide(GTK_WIDGET(loc_input_window));
}

void hide_cons() {
    gtk_widget_hide(GTK_WIDGET(cons_window));
}

void hide_error() {
    gtk_widget_hide(GTK_WIDGET(error_window));
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

    button = gtk_builder_get_object(builder, "button3");
    g_signal_connect(button, "clicked", G_CALLBACK(get_user_input_fast), NULL);

    button = gtk_builder_get_object(builder, "button4");
    g_signal_connect(button, "clicked", G_CALLBACK(constraint_window_renderer), NULL);

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
    g_signal_connect(button, "clicked", G_CALLBACK(map_back_to_main), NULL);

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
    g_signal_connect(button, "clicked", G_CALLBACK(shortest_back_to_main), NULL);

    gtk_widget_show_all(GTK_WIDGET(shortest_window));
}

void fastest_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/fastest.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    fastest_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (fastest_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(fastest_back_to_main), NULL);

    gtk_widget_show_all(GTK_WIDGET(fastest_window));
}

void constraint_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/constraints.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    cons_window = gtk_builder_get_object(builder, "main_window");
    g_signal_connect (cons_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "location");
    g_signal_connect(button, "clicked", G_CALLBACK(get_input_loc_renderer), NULL);


    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_cons), NULL);

    gtk_widget_show_all(GTK_WIDGET(cons_window));
}

void loc_shortest_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/location.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    loc_shortest_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (loc_shortest_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(location_back_to_main), NULL);

    gtk_widget_show_all(GTK_WIDGET(loc_shortest_window));
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

void get_input_fast_renderer() {
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
    g_signal_connect(button, "clicked", G_CALLBACK(deal_fast_input), NULL);

    gtk_widget_show_all(GTK_WIDGET(input_window));
}

void get_loc_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/loc_input.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    input_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (input_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_input), NULL);

    entry = gtk_builder_get_object(builder, "entry_input");
    button = gtk_builder_get_object(builder, "next_button");
    g_signal_connect(button, "clicked", G_CALLBACK(deal_loc_input), NULL);

    gtk_widget_show_all(GTK_WIDGET(input_window));
}

void get_input_loc_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/input.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    loc_input_window = gtk_builder_get_object (builder, "main_window");
    g_signal_connect (loc_input_window, "destroy", G_CALLBACK (window_quit), NULL);

    // back button
    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_loc_input), NULL);

    // next button
    entry = gtk_builder_get_object(builder, "entry_input");
    button = gtk_builder_get_object(builder, "next_button");
    g_signal_connect(button, "clicked", G_CALLBACK(deal_s_e_loc_input), NULL);

    gtk_widget_show_all(GTK_WIDGET(loc_input_window));
}

void error_window_renderer() {
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "./ui/error.xml", &error) == 0) {
        g_printerr("Error loading ui file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    error_window = gtk_builder_get_object (builder, "invalid_window");
    g_signal_connect (error_window, "destroy", G_CALLBACK (window_quit), NULL);

    button = gtk_builder_get_object(builder, "back_button");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_error), NULL);

    gtk_widget_show_all(GTK_WIDGET(error_window));
}