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

    // Adj_list* adj = get_adj_list(adj_table, 54060411);
    // printf("sss\n");
    // while (adj != NULL) {
    //     printf("Neighbor ID: %ld, Length: %f\n", adj->neighbor_node->id, adj->length);
    //     adj = adj->next;
    // }

    dijkstra(&nodes, adj_table, 993673515, 1601125136);

    //draw_edges(&bd, &edges, node_table);

    e_vector_free(&edges);
    n_vector_free(&nodes);
    free_table(node_table);

    return 0;
}