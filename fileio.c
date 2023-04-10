#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"

int read_file(const char* filename, node* nodes, edge* edges, bound* bound_line) {
    FILE* map_file = fopen(filename, "r");
    if (map_file == NULL) {
        perror("map file open");
        return -1;
    }

    // read bound
    char bound[MAX_LINE_LENGTH];
    fgets(bound, MAX_LINE_LENGTH, map_file);
    char* prefix = "<bounding";
    char* suffix = "/bounding>";
    int ret = strncmp(bound, prefix, strlen(prefix));
    if (ret != 0) {
        printf("ERROR: can not read bound line (wrong prefix)\n");
        return -1;
    }

    char* str = strstr(bound, suffix);
    if (str == NULL) {
        printf("ERROR: can not read bound line (wrong suffix)\n");
        return -1;
    }

    // read bound data;
    double min_lat;
    double max_lat;
    double min_lon;
    double max_lon;
    sscanf(bound, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>", &min_lat, &max_lat, &min_lon, &max_lon);
    printf("%lf %lf %lf %lf\n", min_lat, max_lat, min_lon, max_lon);

    return 0;
}