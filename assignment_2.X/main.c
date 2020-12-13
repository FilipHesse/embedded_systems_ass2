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
        // Schedule and execute tasks
        scheduler(&schedInfo[0]);
        
        //wait till end of period
        tmr_wait_period(TIMER1);
    }
}


