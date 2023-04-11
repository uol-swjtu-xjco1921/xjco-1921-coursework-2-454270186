#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#include "script.h"

int main() {
    Bound bd;
    edge_vector edges;
    node_vector nodes;

    e_vector_init(&edges, 10);
    n_vector_init(&nodes, 10);

    read_file("leeds.map", &nodes, &edges, &bd);

    printf("%lf %lf %lf %lf\n", bd.min_lon, bd.min_lat, bd.max_lon, bd.max_lat);
    
    for (int i = 0; i < edges.size; i++) {
        printf("edge %d: %ld\n", i, edges.edges[i].id);
    }

    for (int i = 0; i < nodes.size; i++) {
        printf("node %d: %ld\n", i, nodes.nodes[i].id);
    }

    set_range(&bd);

    system("bash ./plot_data.sh");

    e_vector_free(&edges);
    n_vector_free(&nodes);

    return 0;
}