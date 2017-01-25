#include <processor_include.h>
#include <sysreg.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>
#include "framework.h"

statis int DELAY_SIZE = 3;
static float X[DSP_BLOCK_SIZE];         // temporary input signal
static float Y[DSP_BLOCK_SIZE];         // temporary output signal
static int enable;
static int ledcounter;

void process(int sig)
{

    double w[DSP_BLOCK_SIZE];         // Initial filter coeff.

    for(n=0; n<DSP_BLOCK_SIZE; ++n) {
        w[n] = 0;
    }

    int n;

    // Get a pointer to the current audio block.
    sample_t *audioin  = dsp_get_audio();

    sample_t *audioout = dsp_get_audio();

    // Copy audio from left channel to input buffer. Audio samples are 32 bit
    // fixed-point values in the range [-1, 1] so no additional scaling is required.
    for(n=0; n<DSP_BLOCK_SIZE; ++n) {
        X[n] = audioin[n].left;
    }

    if(enable) {

        // Run input signal through lms
        block_lms(X,Y,w);

    } else {
        // Pass-through.
        memcpy(Y, X, sizeof(Y));
    }

    // Copy output buffer to left and right audio channels.
    for(n=0; n<DSP_BLOCK_SIZE; ++n) {
        audioout[n].left = Y[n];
        audioout[n].right = Y[n];
    }
}

static void keyboard(int sig)
{
    unsigned int keys = dsp_get_keys();

    if(keys & 1) {
        enable = 1;
    } else if(keys & 2) {
        enable = 0;
    }
}

static void timer(int sig)
{
    ledcounter = (ledcounter+1) & 0x3F;
    dsp_set_leds(ledcounter);
}

void block_lms(X, Y, w){

    // Input signal
    u = X;

    //umat=toeplitz(u(k*M:1:(k+1)*M-1),u(k*M:-1:(k-1)*M+1));



    // Delayed signal
    double d[DSP_BLOCK_SIZE];
    for(nn=0; nn<DELAY_SIZE; ++nn) {
        d[nn] = 0;
    }
    for(nn=0; nn<DSP_BLOCK_SIZE-DELAY_SIZE; ++nn) {
        d[nn] = u[nn + DELAY_SIZE];
    }

    // yvec=umat*w;
    double y[DSP_BLOCK_SIZE];
    for(i=0; i<DSP_BLOCK_SIZE-DELAY_SIZE; ++i) {
        y_t = 0;
        for(j=0; j<DSP_BLOCK_SIZE-DELAY_SIZE; ++j) {

               y_t += w[i]*u_m[i][j];
        }
        y[i] = y_t;
    }

    //evec=d-yvec;
    for(nn=0; nn<DELAY_SIZE; ++nn) {
        e[nn] = d[nn]-y[nn];
    }

    //phi=umat.'*evec;
    double phi[DSP_BLOCK_SIZE];
    for(i=0; i<DSP_BLOCK_SIZE-DELAY_SIZE; ++i) {
        phi_t = 0;
        for(j=0; j<DSP_BLOCK_SIZE-DELAY_SIZE; ++j) {

            phi_t += e[i]*u_m[i][j];
        }
        phi[i] = phi_t;
    }

    //w=w+mu*phi;
    for(nn=0; nn<DELAY_SIZE; ++nn) {
        w[nn] = w[nn]+mu*phi[nn];
    }

    Y = e;

}

void main()
{
    memset(S, 0, sizeof(S));

    // Setup the DSP framework
    dsp_init();

    // Register interrupt handlers:
    // SIG_SP1: the audio callback
    // SIG_USR0: the keyboard callback
    // SIG_TMZ: the timer callback
    interrupt(SIG_SP1, process);
    interrupt(SIG_USR0, keyboard);
    interrupt(SIG_TMZ, timer);
    timer_set(9830400, 9830400);
    timer_on();

    // Enable the DSP framework.
    dsp_start();

    // Everything is handled by the interrupt handlers, so just put an empty
    // idle-loop here. If not, the program falls back to an equivalent idle-loop
    // in the run-time library when main() returns.
    for(;;) {
        idle();
    }
}
