#include "hashtable.h"

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

    // insert the node using head inserting
    if (table[index] == NULL) {
        table[index] = new_node;
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
    Node* curr_node = table[index];
    while (curr_node != NULL) {
        if (curr_node->id == id) {
            return curr_node;
        }
        curr_node = curr_node->next;
    }

    // Not Found
    return NULL;
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
}

