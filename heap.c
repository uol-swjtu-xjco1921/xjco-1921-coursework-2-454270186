#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "heap.h"

Heap* create_heap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->data = (Node*)malloc(sizeof(Node) * (capacity + 1));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void destroy_heap(Heap* heap) {
    free(heap->data);
    free(heap);
}

void swap_node(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

int is_empty(Heap* heap) {
    return heap->size == 0;
}

int is_contain(Heap* heap, int64_t node_id) {
    for (int i = 1; i <= heap->size; i++) {
        if (heap->data[i].id == node_id) {
            return 1;
        }
    }

    return 0;
}

void shift_up(Heap* heap, int index) {
    while (index > 1 && heap->data[index].dis < heap->data[index/2].dis) {
        swap_node(&(heap->data[index]), &(heap->data[index/2]));
        index /= 2;
    }
}

void shift_down(Heap* heap, int index) {
    int child = index * 2;
    while (child <= heap->size) {
        if (child + 1 <= heap->size && heap->data[child+1].dis < heap->data[child].dis) {
            child++;
        }
        if (heap->data[index].dis > heap->data[child].dis) {
            swap_node(&(heap->data[index]), &(heap->data[child]));
            index = child;
            child = index * 2;
        } else {
            break;
        }
    }  
}

void heap_push(Heap* heap, Node node) {
    if (heap->size == heap->capacity) {
        printf("heap is full\n");
        return;
    }

    heap->data[++(heap->size)] = node;
    shift_up(heap, heap->size);
}

Node heap_pop(Heap* heap) {
    if (is_empty(heap)) {
        printf("heap is empty\n");
        Node node = {0, 0.0, 0.0, NULL, NULL, 0.0, 0};
        return node;
    }

    Node top = heap->data[1];
    heap->data[1] = heap->data[heap->size--];
    shift_down(heap, 1);
    return top;
}

void heap_update_node(Heap* heap, Node* node) {
    for (int i = 1; i <= heap->size; i++) {
        if (heap->data[i].id == node->id) {
            heap->data[i].dis = node->dis;
            shift_up(heap, i);
            shift_down(heap, i);
            return;
        }
    }   
}