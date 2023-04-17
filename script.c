#include <stdio.h>
#include <string.h>
#include "script.h"

int set_range(Bound* bd) {
    // x_range
    FILE* xscript = fopen("range_x.txt", "w");
    if (xscript == NULL) {
        perror("fopen");
        return -1;
    }

    fprintf(xscript, "%lf:%lf\n", bd->min_lon-0.001, bd->max_lon+0.001);
    fclose(xscript);

    // y_range
    FILE* yscript = fopen("range_y.txt", "w");
    if (yscript == NULL) {
        perror("fopen");
        return -1;
    }

    fprintf(yscript, "%lf:%lf\n", bd->min_lat-0.001, bd->max_lat+0.001);
    fclose(yscript);

    return 0;
}