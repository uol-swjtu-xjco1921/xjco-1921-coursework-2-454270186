#include "hashtable.h"
#include "models.h"

void test_hash_table() {
    Vis_node* table[TABLE_SIZE] = { NULL };
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }

    // 测试插入和查找
    vis_insert(table, 10);
    vis_insert(table, 20);
    vis_insert(table, 30);
    vis_insert(table, 40);

    printf("Is node 40 visited? %s\n", is_node_visited(table, 40) ? "Yes" : "No");
    printf("Is node 50 visited? %s\n", is_node_visited(table, 50) ? "Yes" : "No");

    // 测试释放哈希表
    free_vis_table(table);

    printf("Is node 20 visited after freeing the table? %s\n", is_node_visited(table, 20) ? "Yes" : "No");
}