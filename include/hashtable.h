#ifndef _HASH_TABLE
#define _HASH_TABLE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "models.h"

#define TABLE_SIZE 1000

typedef struct _vis_node {
    int64_t node_id;
    struct _vis_node* next;
} Vis_node;

/*
    General func
*/
int hash_func(int64_t id);
void free_table(Node* table[]);
void free_adj_table(Node* table[]);
void free_vis_table(Vis_node* table[]);

/*
    HashTable for <node_id, node>
*/
void insert(Node* table[], int64_t id, double lat, double lon);
Node* search(Node* table[], int64_t id);

/*
    HashTable for <node_id, adj_list>
*/
void adj_insert(Node* table[], int64_t id, int64_t neighber_id, double length);
Adj_list* get_adj_list(Node* table[], int64_t id);

/*
    HashTable for <node_id, pre_node_id>
    Used for path backtracking
*/
void pre_insert(Node* table[], int64_t curr_node_id, int64_t pre_node_id);
int64_t get_pre_node_id(Node* table[], int64_t curr_node_id);

/*
    HashSet for <node_id>
*/
void vis_insert(Vis_node* table[], int64_t node_id);
int is_node_visited(Vis_node* table[], int64_t node_id);

#endif