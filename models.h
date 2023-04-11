#ifndef _MODELS
#define _MODELS
#include <stdint.h>

typedef struct {
    int64_t id;
    double lat;
    double lon;
} Node;

typedef struct {
    int64_t id;
    int64_t from;
    int64_t to;
    int32_t way;
    double length;
    double veg;
    double arch;
    double land;
    int32_t POI;
} Edge;

typedef struct {
    double min_lat;
    double max_lat;
    double min_lon;
    double max_lon;
} Bound;

#endif