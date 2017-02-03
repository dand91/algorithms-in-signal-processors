//
// Created by david on 2017-01-25.
//

#include <stdio.h>
#include <stdlib.h>
//#include <block_lms.h>
//#include <framework.h>

double mu = 0.01;
int DELAY_SIZE = 2;
int print = 1;
int DSP_BLOCK_SIZE = 4;

 void toeplitz_test(void){

     int a[5] = {1,2,3,4,5};
     int b[5] = {5,6,7,8,9};

     int N = sizeof(a)/sizeof(int);

    int a_m[N][N];

    for (int i = 0; i < N; ++i) {

        for (int j = i; j < N; ++j) {
            a_m[i][j] = a[N-1-(j-i)];
        }
        for (int j = 0; j < i; ++j) {
            a_m[i][j] = b[-(j-i)];
        }
    }


    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("  %d",a_m[i][j]);
        }
        printf("\n");
    }
}


void block_lms(){

    if(print){

        int nn;
        int n;
        int j;
        int i;

            // print
        printf("\n**********\nNew iteration\n**********\n");

        printf("u_1_s  %d \n", u_1_s);
        printf("u_2_s  %d \n", u_2_s);
        printf("d_s  %d \n", d_s);


        printf("buffer:\n");
        for ( nn = 0; nn < DSP_BLOCK_SIZE*2 -1; ++nn) {
            printf("  %f", buffer[nn]);
        }
        printf("\n");
        printf("\n");

        printf("u1:\n");
            for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", buffer[(u_1_s + nn) % (DSP_BLOCK_SIZE*2 -1)]);
            }
            printf("\n");
            printf("\n");

        printf("u2:\n");
        for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
            printf("  %f", buffer[(u_2_s + nn) % (DSP_BLOCK_SIZE*2 -1)]);
        }
        printf("\n");
        printf("\n");

        printf("d:\n");
            for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", buffer[(d_s + nn) % (DSP_BLOCK_SIZE*2 -1)]);
            }
            printf("\n");
            printf("\n");
                // end print
    }

    // yvec=umat*w;
    double y[DSP_BLOCK_SIZE];
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {

        double y_t = 0;

        for ( j = i; j < DSP_BLOCK_SIZE; ++j) {
            y_t += w[j] * buffer[ (u_1_s + DSP_BLOCK_SIZE-1-(j-i)) % (DSP_BLOCK_SIZE*2 -1)];
        }
        for ( j = 0; j < i; ++j) {
            y_t += w[j] * buffer[ ( u_2_s + -(j-i)) % (DSP_BLOCK_SIZE*2 -1)];
        }

        y[i] = y_t;

    }

    if(print){
            // print
            printf("y: ");
            for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", y[nn]);
            }
            printf("\n");
            // end print
    }
    //evec=d-yvec;
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        e[nn] = buffer[(d_s + nn)  % (DSP_BLOCK_SIZE*2 -1) ] - y[nn];
    }
    if(print){
            // print
            printf("e: ");
            for( nn=0; nn<DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", e[nn]);
            }
            printf("\n");
            // end print
    }
    //phi=umat.'*evec;
    double phi[DSP_BLOCK_SIZE];




    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        int phi_t = 0;

        for ( j = i; j < DSP_BLOCK_SIZE; ++j) {
            phi_t += buffer[ (u_1_s + DSP_BLOCK_SIZE-1-(j-i)) % (DSP_BLOCK_SIZE*2 -1)] * e[j];

        }
        for ( j = 0; j < i; ++j) {
            phi_t += buffer[ ( u_2_s + -(j-i)) % (DSP_BLOCK_SIZE*2 -1)] * e[j];

        }
        phi[i] = phi_t;
    }


    if(print){
            // print
            printf("phi: ");
            for( nn=0; nn<DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", phi[nn]);
            }
            printf("\n");
            // end print
    }
    //w=w+mu*phi;
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        w[nn] = w[nn] + mu * phi[nn];
    }

    if(print){
            // print
            printf("w: ");
            for( nn=0; nn<DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", w[nn]);
            }
            printf("\n");
            // end print
    }
}

int main(){


//    toeplitz_test();

    double X_v[16] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0};

    int u_1_s = 0;
    int u_2_s = DSP_BLOCK_SIZE - 1;
    int d_s = u_2_s - DELAY_SIZE ;

    double buffer[7] = {1, 2, 3, 4, 5 ,6 ,7};

    double Y[DSP_BLOCK_SIZE];
    double w[DSP_BLOCK_SIZE];

    int counter = DSP_BLOCK_SIZE ;

    int k;
    int i;

    for ( k = 0; k < 3; ++k) {

        if(k == 0){


        }else{

            for( i=0; i<DSP_BLOCK_SIZE; ++i) {

                counter++% (DSP_BLOCK_SIZE*2 -1);
                buffer[(u_2_s  + i) % (DSP_BLOCK_SIZE*2 -1)] = X_v[counter];

            }
        }

        block_lms();

        u_1_s = (u_1_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);
        u_2_s = (u_2_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);
        d_s = (d_s +  DSP_BLOCK_SIZE) % (DSP_BLOCK_SIZE*2 -1);


    }

}