//
// Created by david on 2017-01-25.
//

#include <stdio.h>

int main() {


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

    return 0;
}