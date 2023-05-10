#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "log.h"
#include "fileio.h"
#include "errhandler.h"
#include "logger.h"

int64_t last_link_id = 0;

/*
    FILE Read
*/
int read_file(const char* filename, Node* node_table[], Node* adj_table[], edge_vector* edges, node_vector* nodes, Bound* bd){
    FILE* map_file = fopen(filename, "r");
    if (map_file == NULL) {
        //perror("map file open");
        log_error("Failed while open file %s", filename);
        exit(-1);
    }

    char buf[MAX_LINE_LENGTH];
    int ret = -1;
    int line_cnt = 1;
    while (fgets(buf, MAX_LINE_LENGTH, map_file)) {
        DATA_TYPE type = check_type(buf);
        //printf("type is %d\n", type);
        switch (type) {
            case BOUND:
                ret = read_bound(buf, bd);
                if (ret != 0) {
                    return ret;
                }
                break;

            case LINK:
                ret = read_edge(buf, edges, adj_table);
                if (ret != 0) {
                    return ret;
                }
                break;
            
            case NODE:
                ret = read_node(buf, nodes, node_table);
                if (ret != 0) {
                    return ret;
                }
                break;
            
            case SPEED:
                // TODO
                break;

            default:
                //log_warn("Unknown type at line %d in file %s", line_cnt, filename);
        }

        line_cnt++;
    }

    return 0;
}

DATA_TYPE check_type(const char* buf) {
    char substr[20] = {0};
    sscanf(buf, "%[^ ]", substr);

    if (strcmp(substr, "<bounding") == 0) return BOUND;
    if (strcmp(substr, "<link") == 0) return LINK;
    if (strcmp(substr, "<node") == 0) return NODE;
    if (strcmp(substr, "<speed") == 0) return SPEED;

    return -1;
}

int read_bound(const char* buf, Bound* bd) {
    char* prefix = "<bounding";
    char* suffix = "/bounding>";

    int ret = check_line_fmt(buf, prefix, suffix);
    if (ret == WRONG_PREFIX) {
        log_error("Can not read line (wrong prefix)");
        return -1;
    } else if (ret == WRONG_SUFFIX) {
        log_error("Can not read line (wrong suffix)");
        return -1;
    }

    // read data
    sscanf(buf,
           "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>",
           &bd->min_lat, &bd->min_lon, &bd->max_lat, &bd->max_lon);

    return 0;
}

int read_edge(const char* buf, edge_vector* edges, Node* adj_table[]) {
    char* prefix = "<link";
    char* suffix = "/link>";

    int ret = check_line_fmt(buf, prefix, suffix);
    if (ret == WRONG_PREFIX) {
        log_error("Can not read line (wrong prefix)");
        return -1;
    } else if (ret == WRONG_SUFFIX) {
        log_error("Can not read line (wrong suffix)");
        return -1;
    }

    Edge edge;
    sscanf(buf,
           "<link id=%ld node=%ld node=%ld way=%d length=%lf veg=%lf arch=%lf land=%lf POI=%d,;/link>",
           &edge.id, &edge.from, &edge.to, &edge.way, &edge.length, &edge.veg, &edge.arch, &edge.land, &edge.POI);
    last_link_id = (labs(edge.id) >= last_link_id) ? labs(edge.id) : last_link_id;
    adj_insert(adj_table, edge.from, edge.to, edge.length);
    adj_insert(adj_table, edge.to, edge.from, edge.length);
    e_vector_push_back(edges, edge);

    return 0;
}

int read_node(const char* buf, node_vector* nodes, Node* node_table[]) {
    char* prefix = "<node";
    char* suffix = "/node>";

    int ret = check_line_fmt(buf, prefix, suffix);
    if (ret == WRONG_PREFIX) {
        log_error("Can not read line (wrong prefix)");
        return -1;
    } else if (ret == WRONG_SUFFIX) {
        log_error("Can not read line (wrong suffix)");
        return -1;
    }

    int64_t id = -1;
    double lat = -1, lon = -1;
    sscanf(buf,
           "<node id=%ld lat=%lf lon=%lf /node>\n",
           &id, &lat, &lon);
    assert(id != -1 && lat != -1 && lon != -1);
    
    Node node;
    node.id = id;
    node.lat = lat;
    node.lon = lon;
    node.adj_list = NULL;
    node.next = NULL;
    node.dis = 0;
    n_vector_push_back(nodes, node);
    insert(node_table, id, lat, lon);

    return 0;
}

/* 
    FILE Write
*/
int add_link(const char* filename) {
    if (access(filename, F_OK) != 0) {
        log_error("File does not exist");
        return -1;
    }

    FILE* add_link_fd = fopen(filename, "a");
    fseek(add_link_fd, 0, SEEK_END);

    char data_buf[128] = {0};
    int64_t node1_id, node2_id;
    double length;
    printf("Enter node 1 id: ");
    scanf("%ld", &node1_id);
    printf("Enter node 2 id: ");
    scanf("%ld", &node2_id);
    printf("Enter link length: ");
    scanf("%lf", &length);
    fflush(stdin);

    int ret = sprintf(data_buf, 
                      "<link id=-%ld node=%ld node=%ld way=0.0 length=%lf veg=0.0 arch=0.0 land=0.0 POI=,;/link>\n",
                      ++last_link_id, node1_id, node2_id, length);
    if (ret <= 0) {
        log_error("Failed while write into data buffer");
    }

    fputs(data_buf, add_link_fd);
    fflush(add_link_fd);
    INFO("New link is successfully added!");

    fclose(add_link_fd);
    return 0;
}

int add_speed_lim(const char* filename) {
    if (access(filename, F_OK) != 0) {
        log_error("File does not exist");
        return -1;
    }

    FILE* add_speed_fd = fopen(filename, "a");
    fseek(add_speed_fd, 0, SEEK_END);

    
}