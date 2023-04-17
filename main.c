#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#include "script.h"
#include "hashtable.h"
#include "drawer.h"

int main() {
    Bound bd;
    edge_vector edges;
    Node* node_table[TABLE_SIZE] = {NULL};

    e_vector_init(&edges, 10);

    read_file("leeds.map", node_table, &edges, &bd);
    
    set_range(&bd);
    input_edges(&edges, node_table);

    system("bash ./plot_data.sh");

    e_vector_free(&edges);

    return 0;
}