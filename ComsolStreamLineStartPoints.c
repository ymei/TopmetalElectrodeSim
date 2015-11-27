/* Generate starting point coordinates for COMSOL streamlines */
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define ABS(x) ((x)>0?(x):(-(x)))
typedef enum {
    AREA_RECTANGULAR = 0,
    AREA_HEXAGONAL = 1
} AREA_SHAPE;

int main(int argc, char **argv)
{
    AREA_SHAPE areaShape = AREA_HEXAGONAL;
    double dimX, dimY, dx, dy, x, y;
    size_t denX, denY, nx;
    ssize_t i, ix, iy, yl;
    if(argc < 6) {
        fprintf(stderr, "%s AreaShape DenX DenY DimX DimY\n", argv[0]);
        fprintf(stderr, "Density: number of points per linear Dim\n");
        fprintf(stderr, "AreaShape: 0 = RECTANGULAR\n");
        fprintf(stderr, "           1 = HEXAGONAL\n");
        return EXIT_FAILURE;
    }
    areaShape = atol(argv[1]);
    denX = atol(argv[2]);
    denY = atol(argv[3]);
    dimX = atof(argv[4]);
    dimY = atof(argv[5]);

    switch(areaShape) {
    case AREA_RECTANGULAR:
        break;
    case AREA_HEXAGONAL:
        dx = dimX / (double)(denX - 1);
        // dy = dimY / (double)(denY - 1);
        dy = dx * sqrt(3.0)/2.0; denY = 2 * denX - 1; // assuming `regular' hexagon
        yl = (denY-1)/2.0;
        for(i=0; i<2; i++) {
            for(iy = -yl; iy <= yl; iy++) {
                nx = denY - ABS(iy);
                for(ix=0; ix<nx; ix++) {
                    y = dy * iy;
                    x = dx * (ix - (nx-1.0)/2.0);
                    if(i==0) {
                        printf("%g, ", x);
                    } else {
                        printf("%g, ", y);
                    }
                }
            }
            printf("\n\n");
        }
        break;
    }
    
    return EXIT_SUCCESS;
}
