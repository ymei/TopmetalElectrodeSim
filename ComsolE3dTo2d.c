/* convert comsol x y z E output to x y E for splot w pm3d */
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define NX 200
#define NY 200

double xmin = -0.005, xmax = 0.005, ymin = -0.013, ymax = 0.015;
double x, y, z, t;
double Z[NX][NY] = {{0.0}};
unsigned int N[NX][NY] = {{0}};



int main(int argc, char **argv)
{
    FILE * fp;
    unsigned int i=0, j, ix, iy;
    char s[1000];
    
    if ((fp = fopen(argv[1], "r"))==NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
    while(!feof(fp)) {
        fscanf(fp, "%lf%lf%lf%lf\n", &t, &x, &y, &z);
//        printf("%g %g %g\n", x, y, z);
        i++;
        if((xmin < x) && (x < xmax) && (ymin < y) && (y < ymax)) {
            ix = ((x - xmin) / (xmax - xmin) * (double)NX);
            iy = ((y - ymin) / (ymax - ymin) * (double)NY);
            Z[ix][iy] += z;
            N[ix][iy] ++;
        }
    }
//    printf("i = %d\n", i);
    for(i=0; i<NX; i++) {
        for(j=0; j<NY; j++) {
            printf("%g %g %g\n", xmin + (xmax-xmin)/(double)NX * (double)i,
                   ymin + (ymax-ymin)/(double)NY * (double)j,
                   Z[i][j]/(double)((N[i][j]>0)?(N[i][j]):1));
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}
