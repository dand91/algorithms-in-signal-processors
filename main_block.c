#include <processor_include.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#include <filter.h>

#include "framework.h"

static int filter_size = 70;
static int delay = 70;

static double pm w[70];

static double pm e[70];

static double pm y[70];

static double pm buffer[70*2];

double mu = 1.5;

int index_i = 0;
int index_d = 0;

int nn;
int n;
int j;
int i;

int pos = 0;

void block_lms(int pos){

    // yvec=umat*w;
    double y[DSP_BLOCK_SIZE];

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        double y_t = 0;
        for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

            if(pos == 0) {

                int index = - (j - i);

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

                y_t += w[j] * buffer[index];

            }else if(pos == 1){

                y_t += w[j] * buffer[( DSP_BLOCK_SIZE - (j - i)) ];

            }
        }
        y[i] = y_t;
    }

    //evec=d-yvec;
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        e[nn] = buffer[( nn) ] - y[nn];
    }

    //phi=umat.'*evec;
    double phi[DSP_BLOCK_SIZE];
    for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {
        int phi_t = 0;
        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            if(pos == 0) {

                int index = - (j - i);

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

                phi_t += buffer[ index] * e[i];

            }else if(pos == 1){

                phi_t += buffer[ ( DSP_BLOCK_SIZE-1-(j-i))] * e[i];
            }
        }

        phi[j] = phi_t;
    }

    //w=w+mu*phi;
    for ( nn = 0; nn < DSP_BLOCK_SIZE; ++nn) {
        w[nn] = w[nn] + mu * phi[nn];
    }

}


void process(int sig){

    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */


    if(pos == 0){

        for (i = 0; i < filter_size; ++i) {

            buffer[i] = u30[i].right;
        }

        pos = 1;

    }else if(pos == 1){

        for (i = 0; i < filter_size; ++i) {

            buffer[filter_size + i] = u30[i].right;
        }

        pos = 0;
    }

    block_lms(pos);

    for (i = 0; i < filter_size; ++i) {

        u32[0].right = e[i];
        u32[0].left  = e[i];
    }
}

static void keyboard(int sig)
{
    unsigned int keys = dsp_get_keys();
    unsigned int leds = 0;

    if(keys & DSP_SW1) {
        leds = 0;
    } else if(keys & DSP_SW2) {
        leds = DSP_D1;

    } else if(keys & DSP_SW3) {
        leds = DSP_D2;
    } else if(keys & DSP_SW4) {
        leds = DSP_D1|DSP_D2;
    }

    dsp_set_leds(leds);
}

static void timer(int sig)
{
}

void main()
{
    /*
    Setup the DSP framework.
    */
    dsp_init();
    dsp_set_leds(DSP_D1 | DSP_D2);

    /*
    Register interrupt handlers:
    SIG_SP1: the audio callback
    SIG_USR0: the keyboard callback
    SIG_TMZ: the timer callback
    */
    interrupt(SIG_SP1, processnlms);
    interrupt(SIG_USR0, keyboard);
    interrupt(SIG_TMZ, timer);
    timer_set(9830400, 9830400);

    /* timer_on(); */

    /*
    Start the framework.
    */
    dsp_start();

    /*
    Everything is handled by the interrupt handlers, so just put an empty
    idle-loop here. If not, the program falls back to an equivalent idle-loop
    in the run-time library when main() returns.
    */
    for(;;) {
        idle();
    }
}



