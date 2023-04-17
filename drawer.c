#include <stdio.h>
#include "drawer.h"
#include "hashtable.h"
#include "models.h"

/// @brief 
/// @param edges 
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