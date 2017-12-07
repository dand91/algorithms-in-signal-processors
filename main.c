#include <processor_include.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#include <filter.h>

#include "framework.h"

static double pm w[DSP_FILTER_SIZE]; // Filter coefficients

static double pm buffer[DSP_FILTER_SIZE]; // Buffer for u, d

int u_s = 0; // (u start) Parameter to keep track of buffer index

double y = 0;  // Holder for y
double e = 0; // Holder for e

int index_i = 0; // Temporary index holder
int index_d = 0; // Temporaty index holder

int i = 0; // Loop parameter

int lmstype = 1; // For when selecting lms type using button

int step = 5;

void processlms(int sig){ // Standard lms

	double mu = 1.5; // Mu for standard lms
	
	y = 0; // Reset parameter
	e = 0; // Reset parameter
	
    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */
    
    buffer[u_s] = u30[0].right;

    // Preform y=w´*uvec

    for ( i = 0; i < DSP_FILTER_SIZE; ++i) {

        if((u_s-i) < 0){

            index_i = (u_s-i) + DSP_FILTER_SIZE;
            
        }else{

            index_i = (u_s-i);

        }

        y += w[i]*buffer[index_i];

    }

    // Preform e=d-y
    
    if((u_s-DSP_DELAY_SIZE) < 0){ 
    	
        index_d = (u_s-DSP_DELAY_SIZE) + DSP_FILTER_SIZE;

    }else{

        index_d = (u_s-DSP_DELAY_SIZE);
    }

    e = buffer[index_d]-y; 
     
    
   // Preform w=w*mu*u*conj(e)
    
   for ( i = 0; i < DSP_FILTER_SIZE; ++i) {
        
        if((u_s-i) < 0){

        	index_i = (u_s-i) + DSP_FILTER_SIZE;
        
    	}else{

        	index_i = (u_s-i);

   		 }

        w[i]=w[i]+ mu*buffer[index_i]*e;
  
    }
    
    u_s = (u_s + 1) % DSP_FILTER_SIZE;
    
    
    
    u32[0].right = e; // Write result 
    u32[0].left  = e; // Write result
  
}

int a = 100; // Protection parameter 

double u_sum; // Holder for the amplitude of u

static double leak = 4/DSP_FILTER_SIZE; // Leaky factor 

void processnlms(int sig)
{
	double mu = step * 10;
	
    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */

    buffer[u_s] = u30[0].right;
     
	y = 0; // Reset parameter
	e = 0; // Reset parameter

    for ( i = 0; i < DSP_FILTER_SIZE; ++i) {

        if((u_s-i) < 0){

            index_i = (u_s-i) + DSP_FILTER_SIZE;
            
        }else{

            index_i = (u_s-i);

        }
        
        u_sum += buffer[index_i]*buffer[index_i];

        y += w[i]*buffer[index_i];

    }


    if((u_s-DSP_DELAY_SIZE) < 0){
    	
        index_d = (u_s-DSP_DELAY_SIZE) + DSP_FILTER_SIZE;

    }else{

        index_d = (u_s-DSP_DELAY_SIZE);
    }

    e = buffer[index_d]-y;
  

  for ( i = 0; i < DSP_FILTER_SIZE; ++i) {
        
    	if((u_s-i) < 0){

            index_i = (u_s-i) + DSP_FILTER_SIZE;
            
        }else{
        	
            index_i = (u_s-i);
        }
       

        w[i]=(1-mu*leak)*w[i] + (mu/(a+u_sum))*buffer[index_i]*e;
        
    }
    
    u_s = (u_s + 1) % DSP_FILTER_SIZE;
        
    u32[0].right = e; 
    u32[0].left  = e;
  
}

void processnolms(int sig)
{

    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */
	
    e = u30[0].right;
        
    u32[0].right = e; 
    u32[0].left  = e;
  
}


void process(int sig){


	if(lmstype == 1){
	
		processnlms(sig);
		
	}else if(lmstype == 0){
	
		processnolms(sig);
		
	}		
}

static void keyboard(int sig)
{
    unsigned int keys = dsp_get_keys();
    unsigned int leds = 0;

    if(keys & DSP_SW1) { // Key 1, swith between lms
    	
        lmstype = (lmstype + 1) % 2;
        
    } else if(keys & DSP_SW2) { // Key 2, reset filter coefficients
     
	    for ( i = 0; i < DSP_FILTER_SIZE; ++i) {
    
	    	w[i] = 0;
    		
    	}
     
    } else if(keys & DSP_SW3) { // Key 3, increase step size
    	
		step = step + 1;
        
    } else if(keys & DSP_SW4) { // Key 4, decrease step size
    
    	step = step - 1;
    	
    }
    
    // Set leds
    dsp_set_leds(0);
    
   	if(lmstype == 1){ 
	
		dsp_set_leds(DSP_D1);
		
	}else if(lmstype == 0){
	
		dsp_set_leds(DSP_D2);
		
	}	
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
    dsp_set_leds(DSP_D1);
 
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




