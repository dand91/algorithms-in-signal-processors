#include <processor_include.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#include <filter.h>

#include "framework.h"

static int filter_size = 70;
static int delay = 70;

int u_s = 0;

static double pm w[70];

static double pm buffer[70];

double y = 0;
double e = 0;

int index_i = 0;
int index_d = 0;

int i = 0;

int lmstype = 1;

void processlms(int sig){

	double mu = 1.5;
	
	y = 0;
	e = 0;
	
    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */
    
    buffer[u_s] = u30[0].right;


    for ( i = 0; i < filter_size; ++i) {

        if((u_s-i) < 0){

            index_i = (u_s-i) + filter_size;
            
        }else{

            index_i = (u_s-i);

        }

        y += w[i]*buffer[index_i];

    }

    if((u_s-delay) < 0){
    	
        index_d = (u_s-delay) + filter_size;

    }else{

        index_d = (u_s-delay);
    }

    e = buffer[index_d]-y; 
     
   for ( i = 0; i < filter_size; ++i) {
        
        if((u_s-i) < 0){

        	index_i = (u_s-i) + filter_size;
        
    	}else{

        	index_i = (u_s-i);

   		 }

        w[i]=w[i]+ mu*buffer[index_i]*e;
  
    }
    
    u_s = (u_s + 1) % filter_size;
    
    u32[0].right = e; 
    u32[0].left  = e;
  
}

int a = 100;

double u_sum;

void processnlms(int sig)
{
	double mu = 50;
	
    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */

    
    buffer[u_s] = u30[0].right;
     
    double y = 0;
    double e = 0;

    for ( i = 0; i < filter_size; ++i) {

        if((u_s-i) < 0){

            index_i = (u_s-i) + filter_size;
            
        }else{

            index_i = (u_s-i);

        }
        
        u_sum += buffer[index_i]*2;

        y += w[i]*buffer[index_i];

    }


    if((u_s-delay) < 0){
    	
        index_d = (u_s-delay) + filter_size;

    }else{

        index_d = (u_s-delay);
    }

    e = buffer[index_d]-y;
  

  for ( i = 0; i < filter_size; ++i) {
        
    	if((u_s-i) < 0){

            index_i = (u_s-i) + filter_size;
            
        }else{
        	
            index_i = (u_s-i);
        }
       

        w[i]=w[i] + mu/(a+u_sum)*buffer[index_i]*e;
        
    }
    
    u_s = (u_s + 1) % filter_size;
        
    u32[0].right = e; 
    u32[0].left  = e;
  
}


void processnolms(int sig)
{

    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */
	
        
    u32[0].right = u30[0].right; 
    u32[0].left  = u30[0].left;
  
}


void process(int sig){


	if(lmstype == 1){
	
		processlms(sig);
	
	}else if(lmstype == 2){
	
		processnlms(sig);
		
	}else if(lmstype == 3){
	
		processnolms(sig);
		
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
        
            
    for ( i = 0; i < filter_size; ++i) {
    
    	w[i] = 0;
    		
    }
       
 
    } else if(keys & DSP_SW3) {
        leds = DSP_D2;
        
        lmstype = (lmstype + 1) % 3;
        
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




