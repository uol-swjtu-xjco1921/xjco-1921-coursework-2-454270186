#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "pathfinder.h"
#include "log.h"

void print_path(Node* pre_table[], int64_t start_node_id, int64_t end_node_id) {
    int64_t curr_id = end_node_id;
    int num = 0;
    while (curr_id != start_node_id && num++ <= 20) {
        printf("%ld ", curr_id);
        curr_id = get_pre_node_id(pre_table, curr_id);
        if (curr_id == -1) {
            log_error("there is no path between two points");
            break;
        }
    }
    printf("\n");
    printf("finish..\n");
}

void dijkstra(node_vector* nodes, Node* adj_table[], int64_t start_node_id, int64_t end_node_id) {
    log_info("dijkstra starting...");
    // initialize heap and dis
    Heap* heap = create_heap(nodes->size);
    Node* pre_table[TABLE_SIZE] = {NULL};
    for (int i = 0; i < nodes->size; i++) {
        Node node = nodes->nodes[i];
        Adj_list* adj = get_adj_list(adj_table, node.id);
        while (adj != NULL) {
            Node* node = adj->neighbor_node;
            node->is_visited = 0;
            node->dis = (node->id == start_node_id) ? 0.0 : DBL_MAX;
            if (node->id == start_node_id) {
                heap_push(heap, *node);
                pre_insert(pre_table, node->id, 0);
            }

            adj = adj->next;
        }
    }

    if (is_empty(heap)) {
        //printf("Error: no start node\n");
        log_error("Error: no start node");
        return;
    }

    while (!is_empty(heap)) {
        Node curr_node = heap_pop(heap);
        curr_node.is_visited = 1;
        if (curr_node.id == end_node_id) {
            // reach the end
            //printf("the distance between start node and end node is %lf\n", curr_node.dis);
            log_info("the distance between start node and end node is %lf", curr_node.dis);
            break;
        }
        // printf("%ld node's dis is %lf\n", curr_node.id, curr_node.dis);
        // printf("vis is %d\n", curr_node.is_visited);
        Adj_list* adj = get_adj_list(adj_table, curr_node.id);
        while (adj != NULL) {
            //printf("adj while loop\n");
            Node* neighbor_node = adj->neighbor_node;
            double new_dis = curr_node.dis + adj->length;
            //printf("neighbor %ld node's dis is %lf, vis is %d\n", neighbor_node->id, neighbor_node->dis, neighbor_node->is_visited);
            if (!neighbor_node->is_visited && new_dis < neighbor_node->dis) {
                //printf("sss\n");
                neighbor_node->dis = new_dis;
                heap_push(heap, *neighbor_node);

                pre_insert(pre_table, neighbor_node->id, curr_node.id);
                //printf("%ld node's pre node is %ld\n", neighbor_node->id, get_pre_node_id(pre_table, neighbor_node->id));
            }

            adj = adj->next;
        }
    }
    log_info("dijkstra finished...");

    print_path(pre_table, start_node_id, end_node_id);

    destroy_heap(heap);
}  