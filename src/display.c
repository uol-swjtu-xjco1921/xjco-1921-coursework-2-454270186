#include <stdio.h>
#include "display.h"
#include "logger.h"

void display() {
    printf("\n");
    printf("Please select the following options\n");
    printf("------------------------------------------\n");
    printf("1. Draw the map\n");
    printf("2. Draw the shorest route\n");
    printf("3. Draw the fast route\n");
    printf("4. Draw the shorest route (constrain)\n");
    printf("5. Modify attributes\n");
    printf("6. Exit\n");
    printf("------------------------------------------\n");
}

void display_modify_map() {
    printf("\n");
    printf("Which type of map modification do you want to choose?\n");
    printf("------------------------------------------\n");
    printf("1. Add a new link\n");
    printf("2. Add a new speed limit\n");
    printf("3. Modify a speed limit\n");
    printf("------------------------------------------\n");
}