#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plplot/plplot.h>
#include "log.h"
#include "fileio.h"
#include "hashtable.h"
#include "drawer.h"
#include "errhandler.h"
#include "pathfinder.h"
#include "logger.h"
#include "window.h"
#include "init.h"
#include "unit_test.h"

Bound bd;
edge_vector edges;
node_vector nodes;
node_vector path;
Node* node_table[TABLE_SIZE] = {NULL};
Node* adj_table[TABLE_SIZE] = {NULL};
Node* spd_adj_table[TABLE_SIZE] = {NULL};

char* filename;

int main(int argc, char** argv) {
#if RUN_MODE
    // RUN MODE
    check_cmd_arg(argc, argv);
    filename = argv[1];

    init();

    int ret = read_file(filename, node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    if (ret != 0) {
        log_error("Failed while read map file (%s)", filename);
        return -1;
    }

    window_start();
    return 0;
#else
    init();
    // TEST MODE
    test_edge_vector();
    test_node_vector();
    test_hash_table();
    test_pre_insert_and_get_pre_node_id();
    test_vis_insert_and_is_node_visited();
    test_heap_operations();
    test_bad_label();
    test_bad_attribute();
    test_good_map();

    printf("All tests passed!\n");
    
    clear();
    return 0;
#endif
}