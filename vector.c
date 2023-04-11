#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "models.h"
/*
    vector for edge
*/
void e_vector_init(edge_vector* e_vector, int capacity) {
    e_vector->edges = malloc(capacity * sizeof(Edge));
    e_vector->size = 0;
    e_vector->capacity = capacity;
}

void e_vector_push_back(edge_vector* e_vector, Edge edge) {
    if (e_vector->size == e_vector->capacity) {
        e_vector->capacity *= 2;
        e_vector->edges = realloc(e_vector->edges, e_vector->capacity * sizeof(Edge));
    }

    e_vector->edges[e_vector->size++] = edge;
}

void e_vector_free(edge_vector* e_vector) {
    free(e_vector->edges);
}


/*
    vector for node
*/
void n_vector_init(node_vector* n_vector, int capacity) {
    n_vector->nodes = malloc(capacity * sizeof(Node));
    n_vector->size = 0;
    n_vector->capacity = capacity;
}

void n_vector_push_back(node_vector* n_vector, Node node) {
    if (n_vector->size == n_vector->capacity) {
        n_vector->capacity *= 2;
        n_vector->nodes = realloc(n_vector->nodes, n_vector->capacity * sizeof(Node));
    }

    n_vector->nodes[n_vector->size++] = node;
}

void n_vector_free(node_vector* n_vector) {
    free(n_vector->nodes);
}
