//
// Created by david on 2017-01-25.
//

#include <stdio.h>


double mu = 0.9;
int DSP_BLOCK_SIZE = 4;
int DELAY_SIZE = 1;

 void test_block_lms(void){

    int a[4] = {1,2,3,4};

    int N = sizeof(a)/sizeof(int);

    int a_m[N][N];

    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            a_m[i][j] = a[j-i];
            a_m[j][i] = a[j-i];
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("  %d",a_m[i][j]);
        }
        printf("\n");
    }
}

void block_lms(double u[], double e[], double w[]){

    //umat=toeplitz(u(k*M:1:(k+1)*M-1),u(k*M:-1:(k-1)*M+1));
    double u_m[DSP_BLOCK_SIZE][DSP_BLOCK_SIZE];
    for (int i = 0; i < DSP_BLOCK_SIZE; ++i) {
        for (int j = i; j < DSP_BLOCK_SIZE; ++j) {
            u_m[i][j] = u[j-i];
            u_m[j][i] = u[j-i];
        }
    }

    // Delayed signal
    double d[DSP_BLOCK_SIZE];
    for(int nn=0; nn<DELAY_SIZE; ++nn) {
        d[nn] = 0;
    }
    for(int nn=0; nn<DSP_BLOCK_SIZE-DELAY_SIZE; ++nn) {
        d[nn] = u[nn + DELAY_SIZE];
    }

    // yvec=umat*w;
    double y[DSP_BLOCK_SIZE];
    for(int i=0; i<DSP_BLOCK_SIZE; ++i) {
        int y_t = 0;
        for(int j=0; j<DSP_BLOCK_SIZE; ++j) {

            y_t += w[i]*u_m[i][j];
        }
        y[i] = y_t;
    }

    //evec=d-yvec;

    for(int nn=0; nn<DSP_BLOCK_SIZE; ++nn) {
        e[nn] = d[nn]-y[nn];
    }

    //phi=umat.'*evec;

    double phi[DSP_BLOCK_SIZE];
    for(int i=0; i<DSP_BLOCK_SIZE; ++i) {
        int phi_t = 0;
        for(int j=0; j<DSP_BLOCK_SIZE; ++j) {

            phi_t += e[i]*u_m[i][j];
        }
        phi[i] = phi_t;
    }

    //w=w+mu*phi;
    for(int nn=0; nn<DSP_BLOCK_SIZE; ++nn) {
        w[nn] = w[nn]+mu*phi[nn];
    }

    for(int nn=0; nn<DSP_BLOCK_SIZE; ++nn) {
        printf("  %f", e[nn]);
    }
    printf("\n");

}

int main(){

    //test_block_lms();

    double X[4] = {1.0,2.0,3.0,4.0};
    double Y[DSP_BLOCK_SIZE];
    double w[DSP_BLOCK_SIZE];

    block_lms(X, Y, w);
}