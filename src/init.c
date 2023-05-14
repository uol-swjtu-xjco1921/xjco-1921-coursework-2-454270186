#include "init.h"

void init() {
    memset(&bd, 0, sizeof(Bound));
    e_vector_init(&edges, 10);
    n_vector_init(&nodes, 10);
    memset(&path, 0, sizeof(node_vector));
    for (int i = 0; i < TABLE_SIZE; i++) {
        node_table[i] = NULL;
        adj_table[i] = NULL;
    }
}

void clear() {
    e_vector_free(&edges);
    n_vector_free(&nodes);
    n_vector_free(&path);
    free_table(node_table);
    free_adj_table(adj_table);
}

void refresh() {
    clear();
    init();
    int ret = read_file(filename, node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    if (ret != 0) {
        log_error("Failed while read map file (%s)", filename);
        return;
    }
}