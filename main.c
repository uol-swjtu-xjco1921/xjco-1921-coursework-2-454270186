#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plplot/plplot.h>
#include "log.h"
#include "fileio.h"
#include "script.h"
#include "hashtable.h"
#include "drawer.h"
#include "errhandler.h"
#include "pathfinder.h"
#include "logger.h"
#include "display.h"
#include "window.h"

// Interface for user interaction
void init();
void clear();
void display();
void event_loop();

Bound bd;
edge_vector edges;
node_vector nodes;
node_vector path;
Node* node_table[TABLE_SIZE] = {NULL};
Node* adj_table[TABLE_SIZE] = {NULL};
Node* spd_adj_table[TABLE_SIZE] = {NULL};

char* filename;

int main(int argc, char** argv) {
    check_cmd_arg(argc, argv);
    filename = argv[1];

    init();

    int ret = read_file(filename, node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
    if (ret != 0) {
        log_error("Failed while read map file (%s)", filename);
        return -1;
    }

    for (int i = 0; i < edges.size; i++) {
        int cnt = 0;
        printf("POI = ");
        while (edges.edges[i].POI[cnt] != 0) {
            printf("%d ", edges.edges[i].POI[cnt]);
            cnt++;
        }
        printf("\n");
    }
    return 0;
    window_start();
    return 0;

    event_loop();

    clear();
    return 0;
}

void init() {
    memset(&bd, 0, sizeof(Bound));
    e_vector_init(&edges, 10);
    n_vector_init(&nodes, 10);
    memset(&path, 0, sizeof(node_vector));
    for (int i = 0; i < TABLE_SIZE; i++) {
        node_table[i] = NULL;
        adj_table[i] = NULL;
    }
}

void clear() {
    e_vector_free(&edges);
    n_vector_free(&nodes);
    n_vector_free(&path);
    free_table(node_table);
    free_adj_table(adj_table);
}

void event_loop() {
    int choice = -1;
    int ret = -1;
    display();
    while (scanf("%d", &choice) == 1) {
        switch (choice) {
            case 1:
                ret = draw_edges(&bd, &edges, node_table);
                if (ret != 0) {
                    ERROR("Unexpect internal error");
                    return;
                }
                INFO("Map is successfully drawn in <map.png>!");
                window_start();
                break;
            
            case 2:
                int64_t start, end;
                printf("Enter the start node id and end node id (example: 100 200): ");
                while (scanf("%ld %ld", &start, &end) != 2) {
                    ERROR("Invalid node id");
                    while (getchar() != '\n');
                    printf("Enter again: ");
                }
                path = dijkstra(&nodes, adj_table, node_table, -2502, -2506);
                if (path.size == 0) {
                    ERROR("Unexpect internal error while find shortest route");
                    break;
                }
                ret = draw_shortest_path(&bd, &path, &edges, node_table);
                if (ret != 0) {
                    ERROR("Unexpect internal error");
                    break;
                }
                INFO("Shortest route is successfully drawn in <shortest.png>!");
                window_start_shortest();
                break;

            case 3:
                INFO("Draw fast route");
                break;

            case 4:
                INFO("Draw POI");
                break;

            case 5:
                int op = -1;
                display_modify_map();
                scanf("%d", &op);
                switch (op) {
                    case 1:
                        add_link(filename);
                        break;
                }
                break;

            case 6:
                printf("Bye Bye\n");
                return;

            default:
                ERROR("Invalid option! Choose again: ");
                continue;
        }
        clear();
        init();
        int ret = read_file(filename, node_table, adj_table, spd_adj_table, &edges, &nodes, &bd);
        if (ret != 0) {
            log_error("Failed while read map file (%s)", filename);
            return;
        }
        display();
    }
}