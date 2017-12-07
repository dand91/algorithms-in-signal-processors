
#include <processor_include.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#include <filter.h>

#include "framework.h"

static double pm w[DSP_BLOCK_SIZE];

static double pm e[DSP_BLOCK_SIZE];

static double pm y[DSP_BLOCK_SIZE];

static double pm phi[DSP_BLOCK_SIZE];

static double pm buffer[DSP_BLOCK_SIZE*2];

double mu = 0.01;

int index = 0;

int j;
int i;

int pos = 0;

double y_t = 0;
double phi_t = 0;

void temp(){

}

void block_lms(){


	// yvec=umat*w;

    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        y_t = 0;
        for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {

            if(pos == 0) {

                index = - (j - i);

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

            }else if(pos == 1){
            	
            	index =  DSP_BLOCK_SIZE - (j - i);
            	
            	 if(index < 0){

                    index = index + DSP_BLOCK_SIZE;
                }
            }
            
            y_t += w[j] * buffer[index];
        }
        y[i] = y_t;
    }

    //evec=d-yvec;

        for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
        	
            if(pos == 0){

                index = - DELAY + i;

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

            }else if(pos == 1){

                index = DSP_BLOCK_SIZE - DELAY + i;
                
                if(index < 0){

                    index = index + DSP_BLOCK_SIZE;
                }

            }

              e[i] = buffer[index] - y[i];
        }


    //phi=umat.'*evec;

    for ( j = 0; j < DSP_BLOCK_SIZE; ++j) {
    	
        phi_t = 0;
        
        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            if(pos == 0) {

                index = - (j - i);

                if(index < 0){

                    index = index + DSP_BLOCK_SIZE*2;
                }

            }else if(pos == 1){
            
            	  index = DSP_BLOCK_SIZE-1-(j-i);

                if(index < 0){ 

                    index = index + DSP_BLOCK_SIZE;
                }
            }
            
             phi_t += buffer[index] * e[i];
        }

        phi[j] = phi_t;
    }

    //w=w+mu*phi;
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
    	
        w[i] = w[i] + mu * phi[i];
    }
}

void process(int sig){
    
	sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */


	if(pos == 0){

        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            buffer[i] = u30[i].right;
        }

        pos = 1;

    }else if(pos == 1){

        for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

            buffer[DSP_BLOCK_SIZE + i] = u30[i].right;
        }

        pos = 0;
    }

    block_lms();

    for (i = 0; i < DSP_BLOCK_SIZE; ++i) {

        u32[i].right = e[i];
        u32[i].left  = e[i];
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
        
                    
    for ( i = 0; i < DSP_BLOCK_SIZE; ++i) {
    
    	w[i] = 0;
    		
    }

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
    interrupt(SIG_SP1, process);
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



