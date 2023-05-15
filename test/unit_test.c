#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "log.h"
#include "fileio.h"
#include "hashtable.h"
#include "vector.h"
#include "heap.h"
#include "pathfinder.h"
#include "logger.h"
#include "init.h"

#include "unit_test.h"

// test edge_vector
void test_edge_vector() {
    edge_vector e_vector;
    e_vector_init(&e_vector, 2);

    Edge edge1 = {  };
    e_vector_push_back(&e_vector, edge1);
    assert(e_vector.size == 1);
    assert(e_vector.capacity == 2);

    Edge edge2 = {  };
    e_vector_push_back(&e_vector, edge2);
    assert(e_vector.size == 2);
    assert(e_vector.capacity == 2);

    e_vector_free(&e_vector);
    assert(e_vector.size == 0);
    assert(e_vector.capacity == 0);

    INFO("-----test_edge_vector PASSED-----");
}

// test node_vector
void test_node_vector() {
    node_vector n_vector1;
    n_vector_init(&n_vector1, 2);

    Node node1 = {  };
    n_vector_push_back(&n_vector1, node1);
    assert(n_vector1.size == 1);
    assert(n_vector1.capacity == 2);

    node_vector n_vector2;
    n_vector_init(&n_vector2, 2);

    Node node2 = {  };
    n_vector_push_back(&n_vector2, node2);
    assert(n_vector2.size == 1);
    assert(n_vector2.capacity == 2);

    n_vector_append(&n_vector1, &n_vector2);
    assert(n_vector1.size == 2);
    assert(n_vector1.capacity == 2);

    n_vector_free(&n_vector1);
    assert(n_vector1.size == 0);
    assert(n_vector1.capacity == 0);

    INFO("-----test_node_vector PASSED-----");
}

void test_hash_table() {
    Node* table[TABLE_SIZE] = { NULL };

    // insert
    insert(table, 1, 10.0, 20.0);
    insert(table, 2, 30.0, 40.0);
    insert(table, 3, 50.0, 60.0);

    // search
    Node* node = search(table, 2);
    assert(node != NULL);
    assert(node->id == 2);
    assert(node->lat == 30.0);
    assert(node->lon == 40.0);
    Node* node1 = search(table, 5);
    assert(node1 == NULL);
    
    INFO("-----test_hash_table PASSED-----");
}

void test_pre_insert_and_get_pre_node_id() {
    Node* table[TABLE_SIZE] = { NULL };

    // insert
    pre_insert(table, 1, 100);
    pre_insert(table, 2, 200);
    pre_insert(table, 3, 300);

    // get pre node
    int64_t pre_node_id = get_pre_node_id(table, 3);
    assert(pre_node_id == 300);
    pre_node_id = get_pre_node_id(table, 2);
    assert(pre_node_id == 200);

    INFO("-----test_pre_table PASSED-----");
}

void test_vis_insert_and_is_node_visited() {
    Vis_node* table[TABLE_SIZE] = { NULL };

    // insert
    vis_insert(table, 1);
    vis_insert(table, 2);
    vis_insert(table, 3);

    // check visited
    int visited = is_node_visited(table, 2);
    assert(visited == 1);
    visited = is_node_visited(table, 5);
    assert(visited == 0);

    free_vis_table(table);
    visited = is_node_visited(table, 2);
    assert(visited == 0);

    INFO("-----test_vis_table PASSED-----");
}

void test_heap_operations() {
    // create
    int capacity = 10;
    Heap* heap = create_heap(capacity);

    // insert
    Node node1 = { 2, 10.0, 20.0, NULL, NULL, 10, -1 };
    heap_push(heap, node1);
    Node node2 = { 3, 30.0, 40.0, NULL, NULL, 100, -1 };
    heap_push(heap, node2);
    Node node3 = { 4, 50.0, 60.0, NULL, NULL, 444, -1 };
    heap_push(heap, node3);

    // pop
    Node poppedNode = heap_pop(heap);
    assert(poppedNode.id == 2 && poppedNode.lat == 10.0 && poppedNode.lon == 20.0);

    // update
    Node pushNode = { 1, 30.0, 40.0, NULL, NULL, 5, -1 };
    heap_push(heap, pushNode);
    poppedNode = heap_pop(heap);
    assert(poppedNode.id == 1 && poppedNode.lat == 30.0 && poppedNode.lon == 40.0);

    // is empty
    assert(is_empty(heap) == 0);

    // is contain
    assert(is_contain(heap, 4) == 1);

    // destroy
    destroy_heap(heap);
}

void test_bad_label() {
    int ret = read_file("./test/test_data/bad_label.map", node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    assert(ret == -1);
}

void test_bad_attribute() {
    int ret = read_file("./test/test_data/bad_attribute.map", node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    assert(ret == -1);
}

void test_good_map() {
    int ret = read_file("./test/test_data/leeds_test.map", node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    assert(ret == 0);

    clear();
    init();
    ret = read_file("./test/test_data/good.map", node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    assert(ret == 0);
}