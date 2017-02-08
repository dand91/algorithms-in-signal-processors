#include <math.h>
#include <matrix.h>
#include <mex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){


    static int DSP_BLOCK_SIZE = 50;
    static int DELAY_SIZE = 48;
    int nn;
    int n;
    int j;
    int i;

    int *u_1_s, *u_2_s, *d_s;
    double *mu;

    mu = mxGetPr(prhs[2]);
    u_1_s = (int *)mxGetPr(prhs[5]);
    u_2_s = (int *)mxGetPr(prhs[6]);
    d_s = (int *)mxGetPr(prhs[7]);


    double buffer[DSP_BLOCK_SIZE], w[DSP_BLOCK_SIZE];

    double *t1,*t2;

    t1 = mxGetPr(prhs[3]);
    t2 = mxGetPr(prhs[4]);

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        w[i] = t2[i];
    }

    for ( i = 0; i < DSP_BLOCK_SIZE*2-1; ++i) {
        buffer[i] = t1[i];
    }


    double y[DSP_BLOCK_SIZE];
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

        double y_t = 0;

        for ( j = i; j < DSP_BLOCK_SIZE; ++j) {

            y_t += w[j] * buffer[ (*u_1_s + DSP_BLOCK_SIZE-1-(j-i)) % (DSP_BLOCK_SIZE*2 -1)];
        }
        for ( j = 0; j < i; ++j) {

            y_t += w[j] * buffer[ ( *u_2_s + -(j-i)) % (DSP_BLOCK_SIZE*2 -1)];
        }

        y[i] = y_t;

    }

    plhs[0] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);
    plhs[1] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);

    double e[DSP_BLOCK_SIZE];

    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
         e[nn] = buffer[(*d_s + nn) % (DSP_BLOCK_SIZE*2 -1)] - y[nn];
    }

    memcpy( mxGetPr(plhs[0]) , e, sizeof(e));

    double phi[DSP_BLOCK_SIZE];
    for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

        double phi_t = 0;

        for (i = j; i < DSP_BLOCK_SIZE; ++i) {
            phi_t += buffer[ (*u_1_s + DSP_BLOCK_SIZE-1-(j-i)) % (DSP_BLOCK_SIZE*2 -1)] * e[j];

        }
        for ( i = 0; i < j; ++i) {
            phi_t += buffer[ (*u_2_s + -(j-i))% (DSP_BLOCK_SIZE*2 -1)] * e[j];

        }
        phi[j] = phi_t;

    }

    double w_t[DSP_BLOCK_SIZE];

    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        w_t[nn] = w[nn] + *mu * phi[nn];
    }

    memcpy( mxGetPr(plhs[1]) , w_t, sizeof(w_t));

}