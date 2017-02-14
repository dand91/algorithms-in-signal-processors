//
// Created by david on 2017-01-25.
//

#include <stdio.h>
#include <stdlib.h>
//#include <block_lms.h>
//#include <framework.h>

double mu = 0.9;
int DELAY_SIZE = 2;
int print = 1;
int DSP_BLOCK_SIZE = 5;

 void toeplitz_test(void){

     int a[9] = {1,2,3,4,5,6,7,8,9};

     int N = 5;

    int a_m[DSP_BLOCK_SIZE][DSP_BLOCK_SIZE];

    for (int j = 0; j < DSP_BLOCK_SIZE; ++j) {
        for (int i = 0; i < DSP_BLOCK_SIZE; ++i) {
            a_m[i][j] = a[DSP_BLOCK_SIZE-1-(i-j)];
        }

    }


    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("  %d",a_m[i][j]);
        }
        printf("\n");
    }
}


void block_lms(double buffer[],double e[],double w[],int pos){


    int nn;
    int n;
    int j;
    int i;

    if(print){

            // print
        printf("\n**********\nNew iteration\n**********\n");

        printf("pos  %d \n", pos);

        printf("buffer:\n");
        for ( nn = 0; nn < DSP_BLOCK_SIZE*2; ++nn) {
            printf("  %f", buffer[nn] );
        }
        printf("\n");
        printf("\n");

        printf("u1:\n");
            for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
                if(pos == 0){
                    printf("  %f", buffer[(DSP_BLOCK_SIZE + nn)]);
                }else if(pos == 1){
                    printf("  %f", buffer[(nn)]);
                }
            }
            printf("\n");
            printf("\n");

        printf("u2:\n");
        for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
            if(pos == 0){
                printf("  %f", buffer[nn]);
            }else if(pos == 1){
                printf("  %f", buffer[(DSP_BLOCK_SIZE + nn) ]);
            }
        }
        printf("\n");
        printf("\n");

        printf("d:\n");
            for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
                if(pos == 0){

                    int index = - DELAY_SIZE + nn;

                    if(index < 0){

                        index = index + DSP_BLOCK_SIZE*2;
                    }

                    printf("  %f", buffer[index]);

                }else if(pos == 1){

                    int index = DSP_BLOCK_SIZE - DELAY_SIZE + nn;

                    printf("  %f", buffer[index]);
                }
            }
            printf("\n");
            printf("\n");

        printf("\n");
        printf("\n");

                // end print
    }

    // yvec=umat*w;
    int u_m[DSP_BLOCK_SIZE][DSP_BLOCK_SIZE];
    int y_m[DSP_BLOCK_SIZE][DSP_BLOCK_SIZE];

    double y[DSP_BLOCK_SIZE];

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        double y_t = 0;
        for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

            if(pos == 0) {

                int index = - (j - i);

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

                u_m[i][j] = buffer[index];
                y_m[i][j] = w[j] * buffer[index];

                y_t += w[j] * buffer[index];

            }else if(pos == 1){

                u_m[i][j] = buffer[(DSP_BLOCK_SIZE - (j - i)) ];
                y_m[i][j] = w[j] * buffer[(DSP_BLOCK_SIZE - (j - i)) ];

                y_t += w[j] * buffer[( DSP_BLOCK_SIZE - (j - i)) ];

            }
        }

        y[i] = y_t;

    }

    if(print){
        // print

            printf("u_m:\n");

            for (int i = 0; i < DSP_BLOCK_SIZE; ++i) {
                for (int j = 0; j < DSP_BLOCK_SIZE; ++j) {
                    printf("  %d",u_m[i][j]);
                }
                printf("\n");
            }


            printf("y_m:\n");

            for (int i = 0; i < DSP_BLOCK_SIZE; ++i) {
                for (int j = 0; j < DSP_BLOCK_SIZE; ++j) {
                    printf("  %d",y_m[i][j]);
                }
                printf("\n");
            }


            printf("y: ");
            for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
                printf("  %f", y[nn]);
            }
            printf("\n");
            // end print
    }
    //evec=d-yvec;
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        e[nn] = buffer[( nn) ] - y[nn];
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

    int u2_m[DSP_BLOCK_SIZE][DSP_BLOCK_SIZE];
    int phi_m[DSP_BLOCK_SIZE][DSP_BLOCK_SIZE];

    double phi[DSP_BLOCK_SIZE];
    for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {
        int phi_t = 0;
        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            if(pos == 0) {

                int index = - (j - i);

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

                u2_m[i][j] =  buffer[ index];
                phi_m[i][j] =  buffer[ index] * e[i];
                phi_t += buffer[ index] * e[i];


            }else if(pos == 1){

                u2_m[i][j] = buffer[ ( DSP_BLOCK_SIZE-1-(j-i))];
                phi_m[i][j] = buffer[ ( DSP_BLOCK_SIZE-1-(j-i))] * e[i];
                phi_t += buffer[ ( DSP_BLOCK_SIZE-1-(j-i))] * e[i];

            }

        }

        phi[j] = phi_t;
    }

    if(print){
        // print

        printf("u_m:\n");

        for (int i = 0; i < DSP_BLOCK_SIZE; ++i) {
            for (int j = 0; j < DSP_BLOCK_SIZE; ++j) {
                printf("  %d",u2_m[i][j]);
            }
            printf("\n");
        }


        printf("phi_m:\n");

        for (int i = 0; i < DSP_BLOCK_SIZE; ++i) {
            for (int j = 0; j < DSP_BLOCK_SIZE; ++j) {
                printf("  %d",phi_m[i][j]);
            }
            printf("\n");
        }


        printf("phi: ");
        for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
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

    int pos = 0;

    toeplitz_test();

    double X_v[20] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0,20.0};

    int u_1_s = 0;
    int u_2_s = DSP_BLOCK_SIZE;
    int d_s = u_2_s - DELAY_SIZE ;

    double Y[DSP_BLOCK_SIZE];
    double w[DSP_BLOCK_SIZE];
    double buffer[DSP_BLOCK_SIZE*2];

    int counter = 0;

    int k;
    int i;

    for ( k = 0; k < 4; ++k) {
            if(pos == 0){
                for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

                    buffer[i] = X_v[counter];
                    counter++;
                }
                block_lms(buffer, Y, w, pos);

                pos = 1;

            }else if(pos == 1){

                for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

                    buffer[DSP_BLOCK_SIZE + i] = X_v[counter];
                    counter++;
                }
                block_lms(buffer, Y, w, pos);

                pos = 0;
            }

    }
}