/*
 * File:   main.c
 * Author: filip,Matteo
 *
 * Created on December 9, 2020, 12:05 AM
 */


#include "xc.h"
#include <stddef.h>
#include "timer.h"
#include "tasks.h"
#include "scheduler.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"



int main() {
    //Setup UART
    uart_config(1, 9600);
    
    //Setup ADC
    adcsetup_temp_volt();
    
    //SetupPWM
    setup_pwm_H2();
    
    //Setup LEDs
    // LED D3
    TRISBbits.TRISB0=0;
    // LED D4
    TRISBbits.TRISB1=0;
    
    //Setup scheduling
    SchedInfo schedInfo[MAX_TASKS];
    configure_tasks_for_scheduling(&schedInfo[0]);
    
    
    while (1) {
        // First task: control
        scheduler(&schedInfo[0]);
       
        
        
        // Current check, does not necesseraly happen every cycle
        // but only when the ADC has been completed(It does not have high priority)
        // If the conversion si done
        if(ADCON1bits.DONE!=0){
            //Set the flag to 0
            ADCON1bits.DONE=0;
            // Read the buffer of the potentiometer
            int potbits=ADCBUF0;
            // Converts the reading in volts
            float potvolts = potbits * 5.0/1024.0;
            float current = 10.0*(potvolts-3.0);
            if(current>15){
                LATBbits.LATB0=1; // turn on the LED
            }else{
                LATBbits.LATB0=0; // turn of the LED
            }
        }
        tmr_wait_period(TIMER1);
    }
}


