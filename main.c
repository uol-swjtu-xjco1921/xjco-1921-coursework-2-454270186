#include <stdio.h>
#include <stdlib.h>
#include <plplot/plplot.h>
#include "log.h"
#include "fileio.h"
#include "script.h"
#include "hashtable.h"
#include "drawer.h"
#include "errhandler.h"

int main(int argc, char** argv) {
    check_cmd_arg(argc, argv);
    char* filename = argv[1];

    Bound bd;
    edge_vector edges;
    Node* node_table[TABLE_SIZE] = {NULL};

    e_vector_init(&edges, 10);

    read_file(filename, node_table, &edges, &bd);

    draw_edges(&bd, &edges, node_table);
    // set_range(&bd);
    // input_edges(&edges, node_table);

    // system("bash ./plot_data.sh");

    e_vector_free(&edges);
    free_table(node_table);

    return 0;
}