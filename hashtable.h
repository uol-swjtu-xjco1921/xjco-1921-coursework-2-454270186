#ifndef _HASH_TABLE
#define _HASH_TABLE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "models.h"

#define TABLE_SIZE 8000

int hash_func(int64_t id);
void insert(Node* table[], int64_t id, double lat, double lon);
Node* search(Node* table[], int64_t id);
void free_table(Node* table[]);

#endif