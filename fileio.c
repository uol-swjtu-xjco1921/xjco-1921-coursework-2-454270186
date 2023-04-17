#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fileio.h"
#include "errhandler.h"

int read_file(const char* filename, Node* node_table[], edge_vector* edges, Bound* bd) {
    FILE* map_file = fopen(filename, "r");
    if (map_file == NULL) {
        perror("map file open");
        exit(-1);
    }

    // read bound
    int ret = read_bound(map_file, bd);
    if (ret != 0) {
        return ret;
    }

    // read edge
    ret = read_edge(map_file, edges);
    if (ret != 0) {
        return ret;
    }

    // read node
    ret = read_node(map_file, node_table);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

int read_bound(FILE* map_file, Bound* bd) {
    if (map_file == NULL) {
        printf("ERROR: map file ptr can not be NULL\n");
        return -1;
    }

    char* prefix = "<bounding";
    char* suffix = "/bounding>";
    char bound_line[MAX_LINE_LENGTH];
    fgets(bound_line, MAX_LINE_LENGTH, map_file);

    int ret = check_line_fmt(bound_line, prefix, suffix);
    if (ret == -1) {
        return -1;
    }

    // read data
    sscanf(bound_line,
           "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>",
           &bd->min_lat, &bd->min_lon, &bd->max_lat, &bd->max_lon);

    return 0;
}

int read_edge(FILE* map_file, edge_vector* edges) {
    if (map_file == NULL) {
        printf("ERROR: map file ptr can not be NULL\n");
        return -1;
    }

    char edge_buf[MAX_LINE_LENGTH];
    char* prefix = "<link";
    char* suffix = "/link>";
    while (fgets(edge_buf, MAX_LINE_LENGTH, map_file)) {
        int ret = check_line_fmt(edge_buf, prefix, suffix);
        if (ret == WRONG_PREFIX) {
            if (strncmp(edge_buf, "<node", strlen("<node")) == 0){
                fseek(map_file, -strlen(edge_buf), SEEK_CUR);
                break;
            }
            
            printf("ERROR: Can not read line (wrong prefix)\n");
            exit(-1);
        } else if (ret == WRONG_SUFFIX) {
            printf("ERROR: Can not read line (wrong suffix)\n");
            exit(-1);
        }

        Edge edge;
        sscanf(edge_buf,
               "<link id=%ld node=%ld node=%ld way=%d length=%lf veg=%lf arch=%lf land=%lf POI=%d,;/link>",
               &edge.id, &edge.from, &edge.to, &edge.way, &edge.length, &edge.veg, &edge.arch, &edge.land, &edge.POI);

        e_vector_push_back(edges, edge);
    }

    return 0;
}

int read_node(FILE* map_file, Node* node_table[]) {
    if (map_file == NULL) {
        printf("ERROR: map file ptr can not be NULL\n");
        return -1;
    }

    char node_buf[MAX_LINE_LENGTH];
    char* prefix = "<node";
    char* suffix = "/node>";
    while (fgets(node_buf, MAX_LINE_LENGTH, map_file)) {
        int ret = check_line_fmt(node_buf, prefix, suffix);
        if (ret == WRONG_PREFIX) {
            char next_line[MAX_LINE_LENGTH];
            fgets(next_line, MAX_LINE_LENGTH, map_file);
            if (check_line_fmt(next_line, prefix, suffix) == 0) {
                printf("ERROR: Can not read line (wrong prefix)\n");
                exit(-1);
            }

            fseek(map_file, -(strlen(next_line)+strlen(node_buf)), SEEK_CUR);
            break;
        } else if (ret == WRONG_SUFFIX) {
            printf("ERROR: Can not read line (wrong suffix)\n");
            exit(-1);
        }

        int64_t id = -1;
        double lat = -1, lon = -1;
        sscanf(node_buf,
               "<node id=%ld lat=%lf lon=%lf /node>\n",
               &id, &lat, &lon);

        assert(id != -1 && lat != -1 && lon != -1);
        insert(node_table, id, lat, lon);
    }

    return 0;
}