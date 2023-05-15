#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "errhandler.h"
#include "log.h"
#include "hashtable.h"

/// @brief check for cmd line arguments
/// @param argc 
/// @param argv 
void check_cmd_arg(int argc, char** argv){
    if (argc != 2) {
        if (argc == 1) {
            printf("Usage: ./build/main <mapfile>\n");
            exit(USAGE);
        }
        printf("ERROR: Bad Argument Count\n");
        exit(BAD_ARG_COUNT);
    }
    if (access(argv[1], F_OK) != 0) {
        printf("ERROR: Bad File Name\n");
        exit(BAD_FILE_NAME);
    }

    log_info("File: %s is valid\n", argv[1]);
}

/// @brief check for data format
/// @param line 
/// @param prefix 
/// @param suffix 
/// @return WRONG_PREFIX or WRONG_SUFFIX if failed, 0 if success
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