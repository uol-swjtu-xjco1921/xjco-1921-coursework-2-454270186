#ifndef _HEAP
#define _HEAP
#include "models.h"

typedef struct _heap {
    Node* data;
    int size;
    int capacity;
} Heap;

Heap* create_heap(int capacity);
void destroy_heap(Heap* heap);

void swap_node(Node* a, Node* b);
int is_empty(Heap* heap);
int is_contain(Heap* heap, int64_t node_id);

void shift_up(Heap* heap, int index);
void shift_down(Heap* heap, int index);
void heap_push(Heap* heap, Node node);
Node heap_pop(Heap* heap);
void heap_update_node(Heap* heap, Node* node);

#endif