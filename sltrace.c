#include <math.h>
#include <string.h>
#include "mex.h"

#ifndef LINE_MAX
#define LINE_MAX 4096
#endif
#define NSL_DEFAULT 1024
#define error_printf mexPrintf

enum mode_types {
    MODE_START_END,
    MODE_LENGTH,
    MODE_HITQ
};

typedef struct sltrace_result
{
    size_t n;
    double *xs, *ys, *zs;
    double *xe, *ye, *ze;
    double *sl; /* length */
} sltrace_result_t;

int sltrace(const char *fname, int mode, double zs, double ze, double r, sltrace_result_t *sr)
{
    FILE *fp = NULL;
    char line[LINE_MAX];
    ssize_t i,ip;
    int slFlag;
    double xp=0.0, yp=0.0, zp=0.0, x, y, z, fid, sll;

    if((fp=fopen(fname,"r"))==NULL) {
        error_printf("Unable to open %s\n", fname);
        return 0;
    }
    sr->n = NSL_DEFAULT;
    sr->xs = realloc(sr->xs, sizeof(double) * sr->n);
    sr->ys = realloc(sr->ys, sizeof(double) * sr->n);
    sr->zs = realloc(sr->zs, sizeof(double) * sr->n);
    sr->xe = realloc(sr->xe, sizeof(double) * sr->n);
    sr->ye = realloc(sr->ye, sizeof(double) * sr->n);
    sr->ze = realloc(sr->ze, sizeof(double) * sr->n);
    sr->sl = realloc(sr->sl, sizeof(double) * sr->n);
    
    ip=-1; i=0; slFlag=0; sll=0.0;
    while(fgets(line, LINE_MAX, fp)!=NULL) {
        if(line[0] == '%') continue; /* comment line */
        sscanf(line, "%lf%lf%lf%lf\n", &x, &y, &z, &fid);
        i=(ssize_t)fid;
        if(i!=ip) { /* found a new streamline */
            if(i>0) {
                if(slFlag) { /* if the previous sl did not satisfy stopping condition */
                    sr->xe[ip] = xp; sr->ye[ip] = yp; sr->ze[ip] = zp;
                }
                if(i >= sr->n) {/* realloc */
                    sr->n *= 2;
                    sr->xs = realloc(sr->xs, sizeof(double) * sr->n);
                    sr->ys = realloc(sr->ys, sizeof(double) * sr->n);
                    sr->zs = realloc(sr->zs, sizeof(double) * sr->n);
                    sr->xe = realloc(sr->xe, sizeof(double) * sr->n);
                    sr->ye = realloc(sr->ye, sizeof(double) * sr->n);
                    sr->ze = realloc(sr->ze, sizeof(double) * sr->n);
                    sr->sl = realloc(sr->sl, sizeof(double) * sr->n);
                }                    
            }
            ip = i;
            sr->xs[i] = xp = x; sr->ys[i] = yp = y; sr->zs[i] = zp = z;
            slFlag = 0;
            sll = 0.0;
        }
        if(z > ze) {
            if(slFlag == 0) { /* edge trigger */
                if(z <= zs) slFlag = 1;
                sr->xs[i] = x; sr->ys[i] = y; sr->zs[i] = z;
            }
        }
        if(slFlag == 1) { /* edge trigger */
            if(z <= ze) slFlag = 0;
            sr->xe[i] = x; sr->ye[i] = y; sr->ze[i] = z;
            sr->sl[i] = sll;
        }
        if(slFlag) {
            sll += sqrt((x-xp)*(x-xp)+(y-yp)*(y-yp)+(z-zp)*(z-zp));
        }
        xp = x; yp = y; zp = z;
    }
    sr->n = i+1;
    if(slFlag) {
        sr->xe[ip] = x; sr->ye[ip] = y; sr->ze[ip] = z;
    }
    
    if(fp) fclose(fp);
    return (int)sr->n;
}

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int errnum = 0;
    char *fname = NULL;
    const mxArray *arg;
    size_t buflen, nrow, ncol;
    ssize_t i, j;
    mxClassID category;
    sltrace_result_t sr;
    double zs=DBL_MAX, ze=-DBL_MAX;
    double *om;

    int mode = MODE_START_END;

    /* check for proper number of arguments */
    if(nrhs<1) {
        errnum = 1; goto out;
    } else {
        arg = prhs[0];
        category = mxGetClassID(arg);
        if(category != mxCHAR_CLASS) {
            errnum = 2; goto out;
        }
    
        buflen = mxGetNumberOfElements(arg) + 1;
        fname = mxCalloc(buflen, sizeof(char));
        if(mxGetString(arg, fname, buflen) != 0) {
            errnum = 3; goto out;
        }
        mexPrintf("Analyzing file %s\n", fname);
    }
    if(nrhs>=4) {
        arg = prhs[1];
        if(!mxIsDouble(arg) || mxIsComplex(arg) || mxGetNumberOfElements(arg)!=1 ) {
            errnum = 4; goto out;
        }
        mode = (int)mxGetScalar(arg);
        arg = prhs[2];
        if(!mxIsDouble(arg) || mxIsComplex(arg) || mxGetNumberOfElements(arg)!=1 ) {
            errnum = 4; goto out;
        }
        zs = mxGetScalar(arg);
        arg = prhs[3];
        if(!mxIsDouble(arg) || mxIsComplex(arg) || mxGetNumberOfElements(arg)!=1 ) {
            errnum = 4; goto out;
        }
        ze = mxGetScalar(arg);
    }
    /* return value */
    if(nlhs>0) {
        if(nlhs!=1) {
            errnum = 5; goto out;
        }
        bzero(&sr, sizeof(sr));
        if(sltrace(fname, mode, zs, ze, 0.0, &sr)) {
            nrow = sr.n;
            ncol = 7;
            plhs[0] = mxCreateDoubleMatrix((mwSize)nrow, (mwSize)ncol, mxREAL);
            om = mxGetPr(plhs[0]);
            j = 0; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.xs[i];
            j = 1; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.ys[i];
            j = 2; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.zs[i];
            j = 3; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.xe[i];
            j = 4; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.ye[i];
            j = 5; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.ze[i];
            j = 6; for(i=0; i<nrow; i++) om[i + j * nrow] = sr.sl[i];
            /* free memory */
            if(sr.xs) free(sr.xs);
            if(sr.ys) free(sr.ys);
            if(sr.zs) free(sr.zs);
            if(sr.xe) free(sr.xe);
            if(sr.ye) free(sr.ye);
            if(sr.ze) free(sr.ze);
            if(sr.sl) free(sr.sl);
        }
    }
    
out:
    if(fname) mxFree(fname);
    switch(errnum) {
    case 1:
        /* this will return from the function back to MATLAB as well */
        mexErrMsgIdAndTxt("TopmetalElectrodeSim:sltrace:nrhs",
                          "At least the streamline data filename is required");
        break;
    case 2:
        mexErrMsgIdAndTxt("TopmetalElectrodeSim:sltrace:fname",
                          "First argument is not a filename string");
        break;
    case 3:
        mexErrMsgIdAndTxt("MATLAB:explore:invalidStringArray",
                          "Could not convert string data");
        break;
    case 4:
        mexErrMsgIdAndTxt("TopmetalElectrodeSim:sltrace:mode",
                          "mode, zs, ze ... parameters must be scalar");
        break;
    case 5:
        mexErrMsgIdAndTxt("TopmetalElectrodeSim:sltrace:nlhs",
                          "Can only return one variable");
        break;
    default:
        break;
    }
}
