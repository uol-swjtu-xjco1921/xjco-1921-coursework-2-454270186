#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "pathfinder.h"
#include "log.h"

node_vector print_path(Node* pre_table[], Node* node_table[], int64_t start_node_id, int64_t end_node_id) {
    node_vector nodes;
    n_vector_init(&nodes, 10);
    int64_t curr_id = end_node_id;
    while (curr_id != 0) {
        printf("%ld ", curr_id);

        Node* node = search(node_table, curr_id);
        if (node == NULL) {
            log_error("no node with id %ld", curr_id);
        }
        n_vector_push_back(&nodes, *node);

        curr_id = get_pre_node_id(pre_table, curr_id);
        if (curr_id == -1) {
            log_error("there is no path between two points");
            break;
        }
    }
    if (curr_id == start_node_id) {
        Node* node = search(node_table, curr_id);
        if (node == NULL) {
            log_error("no node with id %ld", curr_id);
        }
        n_vector_push_back(&nodes, *node);
        printf("%ld ", curr_id);
    }
    printf("\n");
    printf("finish..\n");

    return nodes;
}

node_vector dijkstra(node_vector* nodes, Node* adj_table[], Node* node_table[], int64_t start_node_id, int64_t end_node_id) {
    log_info("dijkstra starting...");
    Heap* heap = create_heap(nodes->size);
    Node* pre_table[TABLE_SIZE] = {NULL};
    Vis_node* vis_table[TABLE_SIZE] = {NULL};

    // initialize heap and dis
    for (int i = 0; i < nodes->size; i++) {
        Node node = nodes->nodes[i];
        Adj_list* adj = get_adj_list(adj_table, node.id);
        while (adj != NULL) {
            Node* node = adj->neighbor_node;
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
        node_vector empty;
        n_vector_init(&empty, 0);
        return empty;
    }

    while (!is_empty(heap)) {
        Node curr_node = heap_pop(heap);
        // curr_node.is_visited = 1;
        vis_insert(vis_table, curr_node.id);
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
            Node* neighbor_node = adj->neighbor_node;
            double new_dis = curr_node.dis + adj->length;

            if (is_node_visited(vis_table, neighbor_node->id) == 0 && new_dis < neighbor_node->dis) {
                printf("neighbor %ld node's dis is %lf\n", neighbor_node->id, neighbor_node->dis);
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

    node_vector path =  print_path(pre_table, node_table, start_node_id, end_node_id);

    destroy_heap(heap);

    return path;
}  