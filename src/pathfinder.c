#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include "pathfinder.h"
#include "log.h"

node_vector print_path(Node* pre_table[], Node* node_table[], int64_t start_node_id, int64_t end_node_id) {
    node_vector nodes;
    n_vector_init(&nodes, 10);
    int64_t curr_id = end_node_id;
    while (curr_id != 0) {
        printf("%ld (pre: %ld)\n", curr_id, get_pre_node_id(pre_table, curr_id));

        Node* node = search(node_table, curr_id);
        if (node == NULL) {
            log_error("no node with id %ld", curr_id);
            exit(-1);
        }
        n_vector_push_back(&nodes, *node);

        curr_id = get_pre_node_id(pre_table, curr_id);
        if (curr_id == -1) {
            log_error("there is no path between two points");
            node_vector empty;
            n_vector_init(&empty, 0);
            return empty;
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

node_vector dijkstra(node_vector* nodes, Node* adj_table[], Node* node_table[], int64_t start_node_id, int64_t end_node_id, double* final_dis) {
    log_info("dijkstra starting...");
    Heap* heap = create_heap(nodes->size);
    Node* pre_table[TABLE_SIZE];
    memset(pre_table, 0, sizeof(pre_table));
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
            log_info("the distance between start node and end node is %lf", curr_node.dis);
            if (final_dis != NULL) {
                *final_dis = curr_node.dis;
            }
            break;
        }

        Adj_list* adj = get_adj_list(adj_table, curr_node.id);
        while (adj != NULL) {
            Node* neighbor_node = adj->neighbor_node;
            double new_dis = curr_node.dis + adj->length;

            if (is_node_visited(vis_table, neighbor_node->id) == 0 && new_dis < neighbor_node->dis) {
                neighbor_node->dis = new_dis;

                int ret = -1;
                if (is_contain(heap, neighbor_node->id)) {
                    ret = heap_update_node(heap, *neighbor_node);
                    log_debug("Updating pre_table: %ld's pre node is set to %ld", neighbor_node->id, curr_node.id);
                } else {
                    heap_push(heap, *neighbor_node);
                    log_debug("pushing pre_table: %ld's pre node is set to %ld", neighbor_node->id, curr_node.id);
                }

                if (ret == 1 || ret == -1) {
                    pre_insert(pre_table, neighbor_node->id, curr_node.id);
                }
            }

            adj = adj->next;
        }
    }
    log_info("dijkstra finished...");

    node_vector path = print_path(pre_table, node_table, start_node_id, end_node_id);

    destroy_heap(heap);
    free_table(pre_table);
    free_vis_table(vis_table);
    return path;
}  

node_vector dijk_poi(node_vector* nodes, Node* adj_table[], Node* node_table[], edge_vector* edges, int target_poi, int64_t start, int64_t end) {
    // find the edge with target POI
    edge_vector poi_edge;
    e_vector_init(&poi_edge, 10);
    for (int i = 0; i < edges->size; i++) {
        for (int j = 0; j < 20; j++) {
            if (edges->edges[i].POI[j] == 0) break;
            if (edges->edges[i].POI[j] == target_poi) {
                Edge temp;
                temp.id = edges->edges[i].id;
                temp.from = edges->edges[i].from;
                temp.to = edges->edges[i].to;
                temp.length = edges->edges[i].length;
                temp.speed = edges->edges[i].speed;
                e_vector_push_back(&poi_edge, temp);
            }
        }
    }

    // triverse all edges with the given poi
    double min_dis = DBL_MAX;
    int min_index = -1;
    node_vector* paths = malloc(sizeof(node_vector) * poi_edge.size);
    for (int i = 0; i < poi_edge.size; i++) {
        double dis_1, dis_2;
        node_vector path_1, path_2;
        n_vector_init(&path_1, 10);
        n_vector_init(&path_2, 10);

        path_1 = dijkstra(nodes, adj_table, node_table, start, poi_edge.edges[i].from, &dis_1);
        path_2 = dijkstra(nodes, adj_table, node_table, poi_edge.edges[i].to, end, &dis_2);
        n_vector_append(&path_1, &path_2);

        paths[i] = path_1;
        if ((dis_1 + dis_2) < min_dis) {
            min_dis = dis_1 + dis_2;
            min_index = i;
        }
    }

    return paths[min_index];
}