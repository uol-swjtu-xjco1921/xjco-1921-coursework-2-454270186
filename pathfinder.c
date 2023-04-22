#include <stdio.h>
#include "pathfinder.h"

void dfs(int64_t current, int64_t end, int64_t* shortest_len, int64_t path[], int path_index, Node* adj_table[], Node* node_table[]) {
    Node* cur_node = search(node_table, current);
    if (cur_node == NULL) {
        printf("Made\n");
        exit(-1);
    }

    cur_node->is_visited = 1;

    path[path_index] = current;
    path_index++;

    if (current == end) {
        if (path_index - 1 < *shortest_len || *shortest_len == -1) {
            *shortest_len = path_index - 1;
        }
    }

    Adj_list* adj = get_adj_list(adj_table, current);
    while (adj != NULL) {
        if (!(adj->neighbor_node->is_visited)) {
            dfs(adj->neighbor_node->id, end, shortest_len, path, path_index, adj_table, node_table);
        }
        adj = adj->next;
    }

    path_index--;
    cur_node->is_visited = 0;
    return ;
}