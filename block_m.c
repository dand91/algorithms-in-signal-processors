#include <math.h>
#include <matrix.h>
#include <mex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){

    static int DSP_BLOCK_SIZE = 10;
    static int delay = 8;
    static double mu = 0.01;
    int i;

    double u[DSP_BLOCK_SIZE], w[DSP_BLOCK_SIZE];
    double *d;
    int *u_s;

    d = mxGetPr(prhs[2]);
    u_s = (int *)mxGetPr(prhs[3]);

    double *t1,*t2;

    t1 = mxGetPr(prhs[0]);
    t2 = mxGetPr(prhs[1]);

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        w[i] = t1[i];
    }

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        u[i] = t2[i];
    }

    double y;
    double e;
    int index;

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

        if((*u_s-i) < 0){

            index = (*u_s-i) + DSP_BLOCK_SIZE;

        }else{

            index = (*u_s-i);

        }

        y += w[i]*u[index];

    }

    int d_index;

    if((*u_s-delay) < 0){

        d_index = (*u_s-delay) + DSP_BLOCK_SIZE;

    }else{

        d_index = (*u_s-delay);

    }

    e = u[d_index]-y;

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

        if((*u_s-i) < 0){

            index = (*u_s-i) + DSP_BLOCK_SIZE;

        }else{

            index = (*u_s-i);

        }

        w[i] = w[i]+mu*u[index]*e;

    }

    plhs[0] = mxCreateDoubleScalar(e);

    plhs[1] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);
    memcpy( mxGetPr(plhs[1]) , w, sizeof(w));


}

void mexFunction2(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){


    static int DSP_BLOCK_SIZE = 5;
    static int DELAY_SIZE = 2;
    static double mu = 0.01;

    int nn;
    int n;
    int j;
    int i;

    int *u_1_s, *u_2_s, *d_s;

    u_1_s = (int *)mxGetPr(prhs[2]);
    u_2_s = (int *)mxGetPr(prhs[3]);
    d_s = (int *)mxGetPr(prhs[4]);

    double buffer[DSP_BLOCK_SIZE], w[DSP_BLOCK_SIZE];

    double *t1,*t2;

    t1 = mxGetPr(prhs[0]);
    t2 = mxGetPr(prhs[1]);

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        w[i] = t2[i];
    }

    for ( i = 0; i < DSP_BLOCK_SIZE*2-1; ++i) {
        buffer[i] = t1[i];
    }

    plhs[0] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);

    double y[DSP_BLOCK_SIZE];
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

        double y_t = 0;

        for ( j = i; j < DSP_BLOCK_SIZE; ++j) {

            y_t += w[j] * buffer[ (*u_1_s + DSP_BLOCK_SIZE-1-(j-i)) % (DSP_BLOCK_SIZE*2 -1)];
        }
        for ( j = 0; j < i; ++j) {

            y_t += w[j] * buffer[ (*u_2_s + -(j-i)) % (DSP_BLOCK_SIZE*2 -1)];
        }

        y[i] = y_t;
    }
    plhs[1] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);

    double e[DSP_BLOCK_SIZE];
    double u_o[DSP_BLOCK_SIZE];

    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
         e[nn] = buffer[(*d_s + nn) % (DSP_BLOCK_SIZE*2 -1)] - y[nn];
         u_o[nn] = buffer[(*u_2_s + nn) % (DSP_BLOCK_SIZE*2 -1)];
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
        w_t[nn] = w[nn] + mu * phi[nn];
    }

    memcpy( mxGetPr(plhs[1]) , w_t, sizeof(w_t));

    *u_1_s = (*u_1_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);
    *u_2_s = (*u_2_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);
    *d_s = (*d_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);


    plhs[2] = mxCreateDoubleScalar(*u_1_s);
    plhs[3] = mxCreateDoubleScalar(*u_2_s);
    plhs[4] = mxCreateDoubleScalar(*d_s);

}