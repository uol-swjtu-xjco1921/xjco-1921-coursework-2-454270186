#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include "log.h"
#include "fileio.h"
#include "errhandler.h"
#include "logger.h"

int64_t last_link_id = 0;

/*
    FILE Read
*/
int read_file(const char* filename, Node* node_table[], Node* adj_table[], Node* spd_adj_table[], edge_vector* edges, node_vector* nodes, Bound* bd){
    FILE* map_file = fopen(filename, "r");
    if (map_file == NULL) {
        printf("ERROR: Bad File Name\n");
        exit(BAD_FILE_NAME);
    }

    char buf[MAX_LINE_LENGTH];
    int ret = -1;
    int line_cnt = 1;
    while (fgets(buf, MAX_LINE_LENGTH, map_file)) {
        if (is_blank_or_empty(buf) == 1) continue;
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
                ret = read_edge(buf, edges, adj_table, spd_adj_table);
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

            case ELSE:
                break;

            default:
                printf("%s\n", buf);
                log_warn("Unknown type at line %d in file %s", line_cnt, filename);
                return -1;
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
    if (strcmp(substr, "<way") == 0) return ELSE;
    if (strcmp(substr, "<geom") == 0) return ELSE;

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

int read_edge(char* buf, edge_vector* edges, Node* adj_table[], Node* spd_table[]) {
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
    edge.id = 0;
    edge.from = edge.to = 0;
    edge.speed = 10;
    edge.length = 0.0;
    memset(edge.POI, 0, sizeof(edge.POI));

    char* token = strtok(buf, "= ");
    while (token != NULL) {
        if (strcmp(token, "<link") == 0) {
            token = strtok(NULL, " ");
            sscanf(token, "id=%ld", &edge.id);
        } else if (strcmp(token, "node") == 0 ) {
            token = strtok(NULL, " ");
            if (edge.from == 0) {
                sscanf(token, "%ld", &edge.from);
            } else {
                sscanf(token, "%ld", &edge.to);
            }
        } else if (strcmp(token, "length") == 0) {
            token = strtok(NULL, " ");
            sscanf(token, "%lf", &edge.length);
        } else if (strcmp(token, "speed") == 0) {
            token = strtok(NULL, " ");
            sscanf(token, "%lf", &edge.speed);
        } else if (strcmp(token, "POI") == 0) {
            int poi_cnt = 0;
            token = strtok(NULL, ",");
            while (token != NULL) {
                sscanf(token, "%d", &edge.POI[poi_cnt++]);

                token = strtok(NULL, ",");
            }
        } else {
            token = strtok(NULL, " ");
        }

        token = strtok(NULL, "=");
    }

    if (edge.id == 0 || edge.from == 0 || edge.to == 0 || edge.length == 0.0) {
        log_error("missing necessary attributes");
        return -1;
    }
    
    // insert length adjacent hashtable
    adj_insert(adj_table, edge.from, edge.to, edge.length);
    adj_insert(adj_table, edge.to, edge.from, edge.length);

    // insert speed adjacent hashtable
    adj_insert(spd_table, edge.from, edge.to, edge.speed);
    adj_insert(spd_table, edge.to, edge.from, edge.speed);

    e_vector_push_back(edges, edge);
    // printf("edge %ld %ld %ld %lf pushed\n", edge.id, edge.from, edge.to, edge.length);
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
    ret = sscanf(buf,
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

int mod_link_attri(const char* filename, int64_t link_id, char* attri_name, double attri_val) {
    if (access(filename, F_OK) != 0) {
        log_error("File does not exist");
        return -1;
    }

    if (check_edge_existence(&edges, link_id) == -1) {
        log_error("link with id [%ld] does not exist", link_id);
        return -1;
    }

    if (strcmp(attri_name, "id") == 0 || strcmp(attri_name, "node") == 0 || 
        strcmp(attri_name, "way") == 0 || strcmp(attri_name, "POI") == 0) {
            log_error("link attribute type [%s] can not be modified", attri_name);
            return -1;
        }
    
    FILE* mod_link_fd = fopen(filename, "r+");
    FILE* temp_fd = tmpfile();

    char target_link[20];
    sprintf(target_link, "%ld", link_id);

    char line[MAX_LINE_LENGTH];
    long cur_pos = ftell(mod_link_fd);
    while (fgets(line, MAX_LINE_LENGTH, mod_link_fd) != NULL) {
        if (check_type(line) != LINK) {
            fputs(line, temp_fd);
            continue;
        }

        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);
        
        if (get_link_id(line_copy) != link_id) {
            cur_pos = ftell(mod_link_fd);
            fputs(line, temp_fd);
            continue;
        }

        char new_attri_val[50];
        sprintf(new_attri_val, "%lf", attri_val);
        char temp_attri_name[20];
        strcpy(temp_attri_name, attri_name);
        strcat(temp_attri_name, "=");
        char* attri_ptr = strstr(line_copy, temp_attri_name);

        if (attri_ptr != NULL) {
            char* value_start = strchr(attri_ptr, '=');

            if (value_start != NULL) {
                value_start++;
                char* value_end;
                if (strcmp(attri_name, "POI") == 0) {
                    value_end = strpbrk(value_start, ";");
                } else {
                    value_end = strpbrk(value_start, " ");
                }
                
                if (value_end != NULL) {
                    int new_val_len = strlen(new_attri_val);
                    int ori_val_len = value_end - value_start;

                    if (new_val_len <= ori_val_len) {
                        strncpy(value_start, new_attri_val, new_val_len);
                    } else {
                        int diff = new_val_len - ori_val_len;
                        memmove(value_end + diff, value_end, strlen(value_end) + 1);
                        strncpy(value_start, new_attri_val, new_val_len);
                    }
                }
            }

        } else if (strcmp(attri_name, "speed") == 0) {
            char* length_attri_ptr = strstr(line_copy, "length=");

            if (length_attri_ptr != NULL) {
                char* insert_pos = strchr(length_attri_ptr, ' ');

                if (insert_pos != NULL) {
                    insert_pos++;
                    char new_attri_line[MAX_LINE_LENGTH];
                    sprintf(new_attri_line, "speed=%lf ", attri_val);
                    size_t insert_pos_offset = insert_pos - line_copy;
                    memmove(insert_pos + strlen(new_attri_line), insert_pos, strlen(line_copy) - insert_pos_offset + 1);
                    strncpy(insert_pos, new_attri_line, strlen(new_attri_line));
                }
            }
        }

        fprintf(temp_fd, "%s", line_copy);
        cur_pos = ftell(mod_link_fd);
    }


    // Copy the contents of the temporary file back to the original file
    rewind(mod_link_fd);
    rewind(temp_fd);
    char temp_line[MAX_LINE_LENGTH];
    while (fgets(temp_line, MAX_LINE_LENGTH, temp_fd) != NULL) {
        fputs(temp_line, mod_link_fd);
    }

    fclose(temp_fd);
    fclose(mod_link_fd);
    return 0;
}