#include <math.h>
#include <matrix.h>
#include <mex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <matrix.h>
#include <mex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){


    static int DSP_BLOCK_SIZE = 30;
    static int DELAY_SIZE = 29;
    static double mu = 0.0;

    int nn;
    int n;
    int j;
    int i;

    int index;

    int *u_1_s, *u_2_s, *d_s;

    u_1_s = (int *)mxGetPr(prhs[2]);
    u_2_s = (int *)mxGetPr(prhs[3]);
    d_s = (int *)mxGetPr(prhs[4]);

    double buffer[DSP_BLOCK_SIZE], w[DSP_BLOCK_SIZE];

    double *t1,*t2;

    t2 = mxGetPr(prhs[1]);

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        w[i] = t2[i];
    }

    t1 = mxGetPr(prhs[0]);

    for ( i = 0; i < DSP_BLOCK_SIZE*2-1; ++i) {
        buffer[i] = t1[i];

    }

    plhs[0] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);
    double y_t;
    double y[DSP_BLOCK_SIZE];
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

         y_t = 0;

        for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

            if((*u_2_s - (j-i)) >= 0){

                index = *u_2_s - (j-i);

            }else{

                index = *u_2_s - (j-i) + (DSP_BLOCK_SIZE*2 -1);
            }


            y_t += w[i] * buffer[index % (DSP_BLOCK_SIZE*2 -1)];

        }

        y[i] = y_t;
    }

    plhs[1] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);

    double e[DSP_BLOCK_SIZE];
    double u_o[DSP_BLOCK_SIZE];

    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        e[nn] = buffer[(*d_s + nn) % (DSP_BLOCK_SIZE*2 -1)]-y[nn];
        u_o[nn] = buffer[(*u_2_s + nn) % (DSP_BLOCK_SIZE*2 -1)];

    }

    memcpy( mxGetPr(plhs[0]) , e, sizeof(e));

    double phi[DSP_BLOCK_SIZE];
    for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

        double phi_t = 0;

        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            if((*u_2_s - (j-i)) >= 0){

                index = *u_2_s - (j-i);

            }else{

                index = *u_2_s - (j-i) + (DSP_BLOCK_SIZE*2 -1);
            }

            phi_t += buffer[index % (DSP_BLOCK_SIZE*2 -1)] * e[j];
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

void mexFunction2(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){


    static int DSP_BLOCK_SIZE = 5;
    static int DELAY_SIZE = 2;
    static double mu = 0.0;

    int nn;
    int n;
    int j;
    int i;

    int index;

    int *u_1_s, *u_2_s, *d_s;

    u_1_s = (int *)mxGetPr(prhs[2]);
    u_2_s = (int *)mxGetPr(prhs[3]);
    d_s = (int *)mxGetPr(prhs[4]);

    double buffer[DSP_BLOCK_SIZE], w[DSP_BLOCK_SIZE];

    double *t1,*t2;

    t1 = mxGetPr(prhs[0]);
    t2 = mxGetPr(prhs[1]);


    mexPrintf("buffer:");

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        w[i] = t2[i];
        buffer[i] = t1[i];

        mexPrintf(" %f", buffer[i] );

    }
    mexPrintf("\n");

    mexPrintf("u:");

    for ( i = 0; i < DSP_BLOCK_SIZE*2-1; ++i) {

        mexPrintf(" %f", buffer[(*u_1_s + i) % (DSP_BLOCK_SIZE*2 -1)]);
    }
    mexPrintf("\n");


    plhs[0] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);


    mexPrintf("u_mat:\n");

    double y[DSP_BLOCK_SIZE];
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

        double y_t = 0;

        for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {


            if(*u_2_s + (j-i) > 0){

                index = *u_2_s + (j-i);

            }else{

                index = *u_2_s + (j-i) + (DSP_BLOCK_SIZE*2 -1);
            }


            y_t += w[i] * buffer[index % (DSP_BLOCK_SIZE*2 -1)];

            mexPrintf("< %d >", (index % (DSP_BLOCK_SIZE*2 -1)));
            mexPrintf(":%d", i);
            mexPrintf("%d:", j);
            mexPrintf("%f", buffer[index % (DSP_BLOCK_SIZE*2 -1)]);

        }

        mexPrintf("\n");


        y[i] = y_t;
    }
    mexPrintf("\n");



    plhs[1] = mxCreateDoubleMatrix(DSP_BLOCK_SIZE,1,mxREAL);

    double e[DSP_BLOCK_SIZE];
    double u_o[DSP_BLOCK_SIZE];

    mexPrintf("d: ");
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
         e[nn] = buffer[(*d_s + nn) % (DSP_BLOCK_SIZE*2 -1)] - y[nn];
         u_o[nn] = buffer[(*u_2_s + nn) % (DSP_BLOCK_SIZE*2 -1)];

         mexPrintf(" %f",buffer[(*d_s + nn) % (DSP_BLOCK_SIZE*2 -1)]);

    }
    mexPrintf("\n");

    memcpy( mxGetPr(plhs[0]) , e, sizeof(e));

    double phi[DSP_BLOCK_SIZE];
    for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

        double phi_t = 0;

        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            if(*u_2_s - (j-i) > 0){

                index = *u_2_s - (j-i);

            }else{

                index = *u_2_s - (j-i) + (DSP_BLOCK_SIZE*2 -1);
            }

            phi_t += buffer[index % (DSP_BLOCK_SIZE*2 -1)] * e[j];

        }

        phi[j] = phi_t;

    }

    mexPrintf("w \n");
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        mexPrintf(" %f ",w[nn]);
    }
    mexPrintf("\n");

    mexPrintf("mu \n");
        mexPrintf(" %f ",mu);
    mexPrintf("\n");


    mexPrintf("phi \n");
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        mexPrintf(" %f ",phi[nn]);
    }
    mexPrintf("\n");


    mexPrintf("w_t \n");

    double w_temp;

    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {

        w_temp = w[nn] + mu * phi[nn];
        w[nn] = w_temp;
        mexPrintf(" %f ",w[nn]);

    }
    mexPrintf("\n");

    memcpy( mxGetPr(plhs[1]) , w, sizeof(w));

    *u_1_s = (*u_1_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);
    *u_2_s = (*u_2_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);
    *d_s = (*d_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);

    plhs[2] = mxCreateDoubleScalar(*u_1_s);
    plhs[3] = mxCreateDoubleScalar(*u_2_s);
    plhs[4] = mxCreateDoubleScalar(*d_s);

    mexPrintf("\n");

}