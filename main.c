#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plplot/plplot.h>
#include "log.h"
#include "fileio.h"
#include "script.h"
#include "hashtable.h"
#include "drawer.h"
#include "errhandler.h"
#include "pathfinder.h"
#include "logger.h"
#include "display.h"
#include "window.h"
#include "init.h"

Bound bd;
edge_vector edges;
node_vector nodes;
node_vector path;
Node* node_table[TABLE_SIZE] = {NULL};
Node* adj_table[TABLE_SIZE] = {NULL};
Node* spd_adj_table[TABLE_SIZE] = {NULL};

char* filename;

int main(int argc, char** argv) {
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
}