#ifndef _MODELS
#define _MODELS
#include <stdint.h>

typedef struct node {
    int64_t id;
    double lat;
    double lon;
} node;

typedef struct edge {
    int64_t id;
    node from;
    node to;
    int32_t way;
    double length;
    double veg;
    double arch;
    int32_t POI;
} edge;

typedef struct bound {
    double min_lat;
    double max_lat;
    double min_lon;
    double max_lon;
} bound;

#endif