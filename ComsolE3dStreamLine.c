/* Trace COMSOL electric field streamline output */

#include <limits.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef LINE_MAX
#define LINE_MAX 4096
#endif

FILE *fp;
char line[LINE_MAX];
double xp, yp, zp, x, y, z, fid;
double z0=0;
double length=0;

int numStreamLines(void);
int hitQ(double x, double y, double z);

int main(int argc, char **argv)
{
    int i, ip;

    if(argc!=2) {
        fprintf(stderr, "%s filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    if((fp=fopen(argv[1],"r"))==NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
#if 0
    fgets(line, LINE_MAX, fp);
    if(line[0]=='%') {
        fgets(line, LINE_MAX, fp);
        sscanf(line, "%lf%lf%lf\n", &xp, &yp, &zp);
        z0=zp;
        printf("z0=%lf\n", z0);
        length=0;
    } else {
        fclose(fp);
        fprintf(stderr, "File format error!\n");
        return EXIT_FAILURE;
    }
    i=1;
#endif
    ip=0; i=0;
    while(fgets(line, LINE_MAX, fp)!=NULL) {
#if 0
        if(i==0) {
            length=0;
            xp=x; yp=y; zp=z;
            i++;
            continue;
        }
#endif
        if(line[0] == '%') continue;
        sscanf(line, "%lf%lf%lf%lf\n", &x, &y, &z, &fid);
        i=(int)fid;
        if(i!=ip) {
            ip = i;
            printf("\n\n");
        }
        fputs(line, stdout);
#if 0
        if(zp==0.04) {
            z0=z;
            printf("z0=%lf ", z0);
            //printf("i=%d, length=%g\n", i, length);
            if(length>1e-6)
                printf("%g\n", length);
            i=0;
            continue;
        }
        if(z>0.02 /*&& (!hitQ(x,y,z)) z>-0.0025 && y>-0.0025 && y<0.0025 && x>-0.0025 && x<0.0025*/ ) {
            length += sqrt((x-xp)*(x-xp)+(y-yp)*(y-yp)+(z-zp)*(z-zp));
        }
        //printf("%lf\t%lf\t%lf\n", x, y, z);
        xp=x; yp=y; zp=z;
        i++;
#endif
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
int hitQ(double x, double y, double z)
{
#define NN 2
    double r=0.000091;
    double z0=0;
    double xc[NN]={-0.001091, 0.001091};
    double yc[NN]={-0.001091, 0.001091};
/*
#define NN 3
    double r=0.0000625;
    double z0=0;
    double xc[NN]={-0.0025, 0, 0.0025};
    double yc[NN]={-0.0025, 0, 0.0025};
*/
    int flag = 0;
    int i;
    
    if(fabs(z-z0)>r) /* out of plane */
        return 0;
    for(i=0; i<NN; i++) {
        flag += ((x-xc[i])*(x-xc[i]) + (z-z0)*(z-z0) <= r*r);
        flag += ((y-yc[i])*(y-yc[i]) + (z-z0)*(z-z0) <= r*r);
    }
    if(flag)
        return 1;
    return 0;
}

