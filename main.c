#include <stdio.h>
#include <stdlib.h>
#include <plplot/plplot.h>
#include "log.h"
#include "fileio.h"
#include "script.h"
#include "hashtable.h"
#include "drawer.h"
#include "errhandler.h"
#include "pathfinder.h"

int main(int argc, char** argv) {
    check_cmd_arg(argc, argv);
    char* filename = argv[1];

    Bound bd;
    edge_vector edges;
    node_vector nodes;
    Node* node_table[TABLE_SIZE] = {NULL};
    Node* adj_table[TABLE_SIZE] = {NULL};

    e_vector_init(&edges, 10);
    n_vector_init(&nodes, 10);

    read_file(filename, node_table, adj_table, &edges,  &nodes,&bd);

    node_vector path = dijkstra(&nodes, adj_table, node_table, 21545960, 664212051);

    //draw_edges(&bd, &edges, node_table);
    draw_shortest_path(&bd, &path, &edges, node_table);

    e_vector_free(&edges);
    n_vector_free(&nodes);
    free_table(node_table);

    return 0;
}