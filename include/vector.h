#ifndef _VECTOR
#define _VECTOR
#include "models.h"

typedef struct {
    Edge* edges;
    int size;
    int capacity;
} edge_vector;

void e_vector_init(edge_vector* e_vector, int capacity);
void e_vector_push_back(edge_vector* e_vector, Edge edge);
void e_vector_free(edge_vector* e_vector);

typedef struct {
    Node* nodes;
    int size;
    int capacity;
} node_vector;

void n_vector_init(node_vector* n_vector, int capacity);
void n_vector_push_back(node_vector* n_vector, Node node);
void n_vector_free(node_vector* n_vector);
void n_vector_append(node_vector* dest, node_vector* src);

#endif