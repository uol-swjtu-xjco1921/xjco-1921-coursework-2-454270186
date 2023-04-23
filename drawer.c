#include <stdio.h>
#include "drawer.h"
#include "hashtable.h"
#include "models.h"
#include "log.h"

/// @brief 
/// @param bd 
/// @param edges 
/// @param table 
/// @return 
int draw_edges(Bound* bd, edge_vector* edges, Node* table[]) {
    plsdev("png");
    plsfnam("map.png");
    plprec(1, 3);
    plinit();
    log_info("plplot initialized");

    plenv(bd->min_lon, bd->max_lon, bd->min_lat, bd->max_lat, 0, 0);
    pllab("", "", "Map");

    for (int i = 0; i < edges->size; i++) {
        double x[2], y[2];
        int64_t from_id = edges->edges[i].from;
        int64_t to_id = edges->edges[i].to;

        Node* from = search(table, from_id);
        if (from == NULL) {
            //printf("can not found node with id %ld\n", from_id);
            log_error("can not found node with id %ld\n", from_id);
            exit(-1);
        }

        Node* to = search(table, to_id);
        if (to == NULL) {
            //printf("can not found node with id %ld\n", to_id);
            log_error("can not found node with id %ld\n", to_id);
            exit(-1);
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
 
int draw_shortest_path(Bound* bd, node_vector* path, edge_vector* edges, Node* table[]) {
    plsdev("png");
    plsfnam("shortest.png");
    plprec(1, 3);
    plinit();
    log_info("plplot initialized");

    plenv(bd->min_lon, bd->max_lon, bd->min_lat, bd->max_lat, 0, 0);
    pllab("", "", "Shortest Route");

    for (int i = 0; i < edges->size; i++) {
        double x[2], y[2];
        int64_t from_id = edges->edges[i].from;
        int64_t to_id = edges->edges[i].to;

        Node* from = search(table, from_id);
        if (from == NULL) {
            //printf("can not found node with id %ld\n", from_id);
            log_error("can not found node with id %ld\n", from_id);
            exit(-1);
        }

        Node* to = search(table, to_id);
        if (to == NULL) {
            //printf("can not found node with id %ld\n", to_id);
            log_error("can not found node with id %ld\n", to_id);
            exit(-1);
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

    log_info("Shortest route drawn successfully");
    plend();
    return 0;
}


/// @brief 
/// @param edges 
/// @param table 
/// @return 
int input_edges(edge_vector* edges, Node* table[]) {
    FILE* edge_file = fopen("edge.dat", "w");
    if (edge_file == NULL) {
        perror("input_edges->fopen()");
        exit(-1);
    }

    for (int i = 0; i < edges->size; i++) {
        int64_t from_id = edges->edges[i].from;
        int64_t to_id = edges->edges[i].to;

        Node* from = search(table, from_id);
        if (from == NULL) {
            printf("can not found node with id %ld\n", from_id);
            
            exit(-1);
        }

        Node* to = search(table, to_id);
        if (to == NULL) {
            printf("can not found node with id %ld\n", to_id);
            exit(-1);
        }

        fprintf(edge_file, "%lf %lf\n", from->lon, from->lat);
        fprintf(edge_file, "%lf %lf\n", to->lon, to->lat);
        fprintf(edge_file, "\n");
    }

    return 0;
}