#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>

// Define a struct to represent a node in the graph
typedef struct node {
    int64_t id;
    char* data;
    struct node* next;
} node_t;

// Define the size of the hash table
#define TABLE_SIZE 100

// Define a function to calculate the hash value of the ID
int hash(int64_t id) {
    return abs(id) % TABLE_SIZE;
}

// Define a function to add a node to the hash table
void insert(node_t* table[], int64_t id, char* data) {
    // Calculate the hash value of the ID
    int index = hash(id);

    // Create a new node to represent the data
    node_t* new_node = malloc(sizeof(node_t));
    new_node->id = id;
    new_node->data = data;
    new_node->next = NULL;

    // Insert the node at the head of the linked list at the hash bucket
    if (table[index] == NULL) {
        table[index] = new_node;
    } else {
        new_node->next = table[index];
        table[index] = new_node;
    }
}

// Define a function to search for a node by ID in the hash table
node_t* search(node_t* table[], int64_t id) {
    // Calculate the hash value of the ID
    int index = hash(id);

    // Traverse the linked list at the hash bucket to find the node with the given ID
    node_t* current = table[index];
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }

    // Return NULL if the node is not found
    return NULL;
}

// Define a function to print the hash table
void print_table(node_t* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Bucket %d: ", i);
        node_t* current = table[i];
        while (current != NULL) {
            printf("(%ld, %s) -> ", current->id, current->data);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main() {
    // Create a hash table with 100 buckets
    node_t* table[TABLE_SIZE] = {NULL};

    // Insert nodes into the hash table
    insert(table, 101, "Node A");
    insert(table, -372166932, "Node B");
    insert(table, 101, "Node C");

    // Print the hash table
    //print_table(table);

    // Search for a node by ID in the hash table
    int64_t search_id = 101;
    node_t* found_node = search(table, search_id);
    if (found_node != NULL) {
        printf("Node with ID %ld found: (%ld, %s)\n", search_id, found_node->id, found_node->data);
    } else {
        printf("Node with ID %ld not found\n", search_id);
    }

    return 0;
}