#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errhandler.h"
#include "log.h"
#include "hashtable.h"

/// @brief 
/// @param argc 
/// @param argv 
void check_cmd_arg(int argc, char** argv){
    if (argc != 2) {
        if (argc == 1) {
            printf("Usage: ./main <mapfile>\n");
            exit(-1);
        }
        fprintf(stderr, "Wrong arguement count\n");
        exit(-1);
    }

    log_info("File: %s is valid\n", argv[1]);
}

/// @brief 
/// @param line 
/// @param prefix 
/// @param suffix 
/// @return 
int check_line_fmt(const char* line, const char* prefix, const char* suffix) {
    // check prefix
    int ret = strncmp(line, prefix, strlen(prefix));
    if (ret != 0) {
        return WRONG_PREFIX;
    }

    // check suffix
    char* str = strstr(line, suffix);
    if (str == NULL) {
        return WRONG_SUFFIX;
    }

    return 0;
}

int check_edge_existence(edge_vector* edges, int64_t link_id) {
    for (int i = 0; i < edges->size; i++) {
        if (edges->edges[i].id == link_id) {
            return 1;
        }
    }

    return -1;
}

int check_node_existence(Node* node_table[], int64_t node_id) {
    Node* node = search(node_table, node_id);
    if (node == NULL) {
        return -1;
    }

    return 1;
}

int check_POI_existence(edge_vector* edges, int POI) {
    for (int i = 0; i < edges->size; i++) {
        for (int j = 0; j < 20; j++) {
            if (edges->edges[i].POI[j] == 0) break;
            if (edges->edges[i].POI[j] == POI) {
                return 1;
            }
        }
    }

    return -1;
}