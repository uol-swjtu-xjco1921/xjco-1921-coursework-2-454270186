#include <stdio.h>
#include "drawer.h"
#include "hashtable.h"
#include "models.h"
#include "log.h"

/// @brief Determine if there are no edges
/// @param edges 
/// @return 
int is_edges_empty(edge_vector* edges) {
    return edges->size == 0;
}

/// @brief draw the whole map
/// @param bd 
/// @param edges 
/// @param table 
/// @return 1 if success, -1 if failed
int draw_edges(Bound* bd, edge_vector* edges, Node* table[]) {
    if (is_edges_empty(edges)) {
        log_error("edges is empty");
        return -1;
    }

    plsdev("png");
    plsfnam("map.png");
    plprec(1, 3);
    plinit();
    log_info("plplot initialized");

    plenv(bd->min_lon-0.001, bd->max_lon+0.001, bd->min_lat-0.001, bd->max_lat+0.001, 0, 0);
    pllab("", "", "Map");

    for (int i = 0; i < edges->size; i++) {
        double x[2], y[2];
        int64_t from_id = edges->edges[i].from;
        int64_t to_id = edges->edges[i].to;

        Node* from = search(table, from_id);
        if (from == NULL) {
            //printf("can not found node with id %ld\n", from_id);
            log_error("can not found node with id %ld\n", from_id);
            return -1;
        }

        Node* to = search(table, to_id);
        if (to == NULL) {
            //printf("can not found node with id %ld\n", to_id);
            log_error("can not found node with id %ld\n", to_id);
            return -1;
        }

        x[0] = from->lon;
        x[1] = to->lon;
        y[0] = from->lat;
        y[1] = to->lat;
        plline(2, x, y);
    }
    
    log_info("Map drawn successfully");
    plend();
    return 0;
}
 
/// @brief draw path
/// @param mode 0 for shortest, 1 for fastest, 2 for POI
/// @return 1 if success, -1 if failed
int draw_path(Bound* bd, node_vector* path, edge_vector* edges, Node* table[], int mode) {
    if (is_edges_empty(edges)) {
        log_error("edges is empty");
        return -1;
    }

    plsdev("png");
    if(mode == 0) {
        plsfnam("shortest.png"); 
    } else if (mode == 1) {
        plsfnam("fastest.png");
    } else if (mode == 2) {
        plsfnam("shortest_poi.png");
    }
    plprec(1, 3);
    plinit();
    log_info("plplot initialized");

    plenv(bd->min_lon-0.001, bd->max_lon+0.001, bd->min_lat-0.001, bd->max_lat+0.001, 0, 0);
    if (mode == 0) {
        pllab("", "", "Shortest Route");
    } else if (mode == 1) {
        pllab("", "", "Fastest Route");
    } else if (mode == 2) {
        pllab("", "", "Shortest Route with POI");
    }

    for (int i = 0; i < edges->size; i++) {
        double x[2], y[2];
        int64_t from_id = edges->edges[i].from;
        int64_t to_id = edges->edges[i].to;

        Node* from = search(table, from_id);
        if (from == NULL) {
            //printf("can not found node with id %ld\n", from_id);
            log_error("can not found node with id %ld\n", from_id);
            return -1;
        }

        Node* to = search(table, to_id);
        if (to == NULL) {
            //printf("can not found node with id %ld\n", to_id);
            log_error("can not found node with id %ld\n", to_id);
            return -1;
        }

        x[0] = from->lon;
        x[1] = to->lon;
        y[0] = from->lat;
        y[1] = to->lat;
        plline(2, x, y);
    }

    int l = 0, r = 1;
    while (l < path->size && r < path->size) {
        double x[2], y[2];
        x[0] = path->nodes[l].lon;
        x[1] = path->nodes[r].lon;
        y[0] = path->nodes[l].lat;
        y[1] = path->nodes[r].lat;
        plcol0(15);
        plline(2, x, y);

        l++;
        r++;
    }

    log_info("%s route drawn successfully", (mode == 0) ? "Shortest" : "Fastest");
    plend();
    return 0;
}

/// @brief draw the shortest path passing given location
/// @return 1 if success, -1 if failed
int draw_loc_path(Bound* bd, node_vector* path_1, node_vector* path_2, edge_vector* edges, Node* table[]) {
    if (is_edges_empty(edges)) {
        log_error("edges is empty");
        return -1;
    }
    
    plsdev("png");
    plsfnam("location.png"); 
    plprec(1, 3);
    plinit();
    log_info("plplot initialized");

    plenv(bd->min_lon-0.001, bd->max_lon+0.001, bd->min_lat-0.001, bd->max_lat+0.001, 0, 0);
    pllab("", "", "Shortest Route");

    for (int i = 0; i < edges->size; i++) {
        double x[2], y[2];
        int64_t from_id = edges->edges[i].from;
        int64_t to_id = edges->edges[i].to;

        Node* from = search(table, from_id);
        if (from == NULL) {
            //printf("can not found node with id %ld\n", from_id);
            log_error("can not found node with id %ld\n", from_id);
            return -1;
        }

        Node* to = search(table, to_id);
        if (to == NULL) {
            //printf("can not found node with id %ld\n", to_id);
            log_error("can not found node with id %ld\n", to_id);
            return -1;
        }

        x[0] = from->lon;
        x[1] = to->lon;
        y[0] = from->lat;
        y[1] = to->lat;
        plline(2, x, y);
    }

    int l = 0, r = 1;
    while (l < path_1->size && r < path_1->size) {
        double x[2], y[2];
        x[0] = path_1->nodes[l].lon;
        x[1] = path_1->nodes[r].lon;
        y[0] = path_1->nodes[l].lat;
        y[1] = path_1->nodes[r].lat;
        plcol0(15);
        plline(2, x, y);

        l++;
        r++;
    }

    l = 0;
    r = 1;
    while (l < path_2->size && r < path_2->size) {
        double x[2], y[2];
        x[0] = path_2->nodes[l].lon;
        x[1] = path_2->nodes[r].lon;
        y[0] = path_2->nodes[l].lat;
        y[1] = path_2->nodes[r].lat;
        plcol0(15);
        plline(2, x, y);

        l++;
        r++;
    }

    log_info("Shortest route (pass given location) drawn successfully");
    plend();
    return 0;
}
