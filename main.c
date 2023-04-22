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
    Node* node_table[TABLE_SIZE] = {NULL};
    Node* adj_table[TABLE_SIZE] = {NULL};

    e_vector_init(&edges, 10);

    read_file(filename, node_table, adj_table, &edges, &bd);

    // Adj_list* adj = get_adj_list(adj_table, 54060411);
    // printf("sss\n");
    // while (adj != NULL) {
    //     printf("Neighbor ID: %ld, Length: %f\n", adj->neighbor_node->id, adj->length);
    //     adj = adj->next;
    // }
    draw_edges(&bd, &edges, node_table);

    e_vector_free(&edges);
    free_table(node_table);

    return 0;
}