#include <assert.h>
#include "hashtable.h"
#include "log.h"

/// @brief calculate the hash value of id
/// @param id the node id
/// @return the hash value
int hash_func(int64_t id) {
    return labs(id) % TABLE_SIZE;
}

/// @brief insert a node into table
/// @param table 
/// @param id 
/// @param lat 
/// @param lon 
void insert(Node* table[], int64_t id, double lat, double lon) {
    int index = hash_func(id);

    Node* new_node = malloc(sizeof(Node));
    new_node->id = id;
    new_node->lat = lat;
    new_node->lon = lon;
    new_node->next = NULL;
    // new_node->is_visited = 0;

    // insert the node using head inserting
    if (table[index] == NULL) {
        table[index] = new_node;
        new_node->next = NULL;
    } else {
        new_node->next = table[index];
        table[index] = new_node;
    }
}

/// @brief find node by given id
/// @param table 
/// @param id 
/// @return the associated node if found, NULL if not found
Node* search(Node* table[], int64_t id) {
    int index = hash_func(id);

    // traverse the linked list in hash table
    Node* cur_node = table[index];
    while (cur_node != NULL) {
        if (cur_node->id == id) {
            return cur_node;
        }
        cur_node = cur_node->next;
    }

    // Not Found
    return NULL;
}

/// @brief find node by given location
/// @param table 
/// @param lat 
/// @param lon 
/// @return the node id if found, 0.0 if not found
int64_t search_by_loc(Node* table[], double lat, double lon) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] == NULL) {
            continue;
        }

        Node* cur_node = table[i];
        while (cur_node != NULL) {
            if (cur_node->lat == lat && cur_node->lon == lon) {
                return cur_node->id;
            }
            cur_node = cur_node->next;
        }
    }

    // not found
    return 0.0;
}

/// @brief 
/// @param table 
void free_table(Node* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] == NULL) {
            continue;
        }

        Node* curr = table[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }

    log_info("table freed");
}

void adj_insert(Node* table[], int64_t id, int64_t neighber_id, double length) {
    int index = hash_func(id);

    Node* cur_node = table[index];
    while (cur_node != NULL && cur_node->id != id) {
        cur_node = cur_node->next;
    }
    if (cur_node == NULL) {
        cur_node = (Node*)malloc(sizeof(Node));
        cur_node->id = id;
        cur_node->adj_list = NULL;
        cur_node->next = table[index];
        table[index] = cur_node;
    }

    Adj_list* adj = (Adj_list*)malloc(sizeof(Adj_list));
    adj->neighbor_node = (Node*)malloc(sizeof(Node));
    adj->neighbor_node->id = neighber_id;
    adj->length = length;
    adj->next = cur_node->adj_list;
    cur_node->adj_list = adj;

    // printf("insert id %ld and neighbor id %ld\n", id, neighber_id);
}

Adj_list* get_adj_list(Node* table[], int64_t id) {
    int index = hash_func(id);

    Node* cur_node = table[index];
    while (cur_node != NULL) {
        if (cur_node->id == id) {
            break;
        }
        cur_node = cur_node->next;
    }

    if (cur_node == NULL) {
        return NULL;
    } else {
        return cur_node->adj_list;
    }
}

void free_adj_table(Node* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] == NULL) {
            continue;
        }

        Node* cur_node = table[i];
        while (cur_node != NULL) {
            Adj_list* adj = cur_node->adj_list;
            while (adj != NULL) {
                Adj_list* next_adj = adj->next;
                free(adj->neighbor_node);
                free(adj);
                adj = next_adj;
            }

            Node* next_node = cur_node->next;
            free(cur_node);
            cur_node = next_node;
        }
    }

    log_info("adj table freed");
}

void pre_insert(Node* table[], int64_t curr_node_id, int64_t pre_node_id) {
    int index = hash_func(curr_node_id);
    Node* pre_node = (Node*)malloc(sizeof(Node));
    pre_node->id = pre_node_id;
    pre_node->next_id = curr_node_id;
    pre_node->next = NULL;
    
    if (table[index] == NULL) {
        table[index] = pre_node;
    } else {
        pre_node->next = table[index];
        table[index] = pre_node;
    }
}

int64_t get_pre_node_id(Node* table[], int64_t curr_node_id) {
    int index = hash_func(curr_node_id);

    // traverse the linked list in hash table
    Node* pre_node = table[index];
    while (pre_node != NULL) {
        if (pre_node->next_id == curr_node_id) {
            //printf("pre_id is: %ld, cur_id is: %ld\n", pre_node->id, curr_node_id);
            return pre_node->id;
        }
        pre_node = pre_node->next;
    }

    // Not Found
    return -1;   
}


void vis_insert(Vis_node* table[], int64_t node_id) {
    int index = hash_func(node_id);

    Vis_node* node = (Vis_node*)malloc(sizeof(Vis_node));
    node->node_id = node_id;
    
    if (table[index] == NULL) {
        table[index] = node;
        node->next = NULL;
    } else {
        node->next = table[index];
        table[index] = node;
    }
}

int is_node_visited(Vis_node* table[], int64_t node_id) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] == NULL) {
            continue;
        }

        Vis_node* n = table[i];

        while (n != NULL) {
            if (n->node_id == node_id) {
                return 1;
            }

            n = n->next;
        }
    }

    return 0;
}

void free_vis_table(Vis_node* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] == NULL) {
            continue;
        }
        
        Vis_node* cur_node = table[i];
        while (cur_node != NULL) {
            Vis_node* tmp_node = cur_node;
            cur_node = cur_node->next;
            free(tmp_node);
        }
        table[i] = NULL;
    }

    log_info("vis table freed");
}
