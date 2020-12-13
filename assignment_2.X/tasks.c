/*
 * File:   tasks.c
 * Author: filip, Matteo
 *
 * Created on December 9, 2020, 10:17 AM
 */


#include "xc.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tasks.h"
#include "timer.h"
#include "parser.h"
#include "ringbuffer.h"
#include "uart.h"
#include "pwm.h"

#define PWM_MAX_VOLTAGE 5.0

void* task1_control(void* params) {
    // Initialize static variables, that will remain their content within this function
    // even after leaving the function and re-entering
    static parser_state parser_ref_speed = {STATE_DOLLAR,
                                            {'\0'},
                                            {'\0'},
                                            0,
                                            0};
    static int ref_speed = 0;
    
    // Check UART ring buffer, if new reference speed was received
    char c; // Character from UART
    while (getUart1CharFromBuffer(&c) > 0) {
        
        //If new message received and message type is correct (MCREF))
        if (parse_byte(&parser_ref_speed, c) == NEW_MESSAGE 
                && strcmp(parser_ref_speed.msg_type, "MCREF") == 0)
        {
            // set new reference speed
            ref_speed = atoi(parser_ref_speed.msg_payload);
        }
    }

    // Compute reference PWM_ratio
    float PWM_ratio;
    if (ref_speed >= 0 && ref_speed <= 1000) {
        PWM_ratio = ref_speed/ 1000.0;
    } 
    else { //Out of range: stop motor!
        PWM_ratio = 0;
    }
    
    // Always send reference voltage to motor
    set_percentage_value_pwm_H2(PWM_ratio);   //from volts to percentage, if dutyCycle = 100%*PWM frequency: output = 5V
    return NULL;
}

void* task2_process_sensors(void* params) {
    // If the conversion si done
    while(ADCON1bits.DONE==0);
    
    //Set the flag to 0
    ADCON1bits.DONE=0;
    
    // Read the buffer of the potentiometer
    int potbits=ADCBUF0;
    
    // Converts the reading in volts
    double potvolts = potbits * 5.0/1024.0;
    double current = 10.0*(potvolts-3.0);
    int int_c=current;
    int dec_c=(current-int_c)*10.0;
    
    // Read the buffer of the temperature
    int tempvolts = ADCBUF1;
    
    // Convert the reading in Celsius Degrees
    float temperature=(tempvolts-0.75)*100.0*25;
    int int_t=temperature;
    int dec_t=(current-int_t)*10.0;
    
    // Compute the message to send applying the protocol
    // Note: an alternative to sprintf() with doubles is used because
    // Definition of the string
    char string0[16]="$MCFBK,";
    char string1[6];
    char string2[6];
    sprintf(string1,"%d.%d",int_c,dec_c);
    sprintf(string2,"%d.%d",int_t,dec_t);
    strcat(string0,string1);
    strcat(string0,",");
    strcat(string0,string2);
    strcat(string0,"*");
    
    // transmit the message via uart2
    uart2TransmitStr(string1); 
    return NULL;
}

void* task3_blink(void* params) {
    // Change of LED D3 state  
    LATBbits.LATB0=!LATBbits.LATB0;
    return NULL;
}

void* task4_check_excess_current(void* params) {
    // Current check, does not necessarily happen every cycle
    // but only when the ADC has been completed(It does not have high priority)
    // If the conversion is done
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
    return NULL;
}

void configure_tasks_for_scheduling(SchedInfo * schedInfo) {
    // Minimal period duration: 5ms; other periods can be divided by 5 ms
    tmr_setup_period(TIMER1, 5); //200 Hz

    // Configure Info of first task
    schedInfo->task = &task1_control;
    schedInfo->params = NULL;
    schedInfo->n = 0;
    schedInfo->N = 1; // each period -> 200 Hz;

    // Configure Info of second task
    schedInfo++;
    schedInfo->task = &task2_process_sensors;
    schedInfo->params = NULL;
    schedInfo->n = 0;
    schedInfo->N = 200; // each 200th period -> 1Hz

    // Configure Info of third task
    schedInfo++;
    schedInfo->task = &task3_blink;
    schedInfo->params = NULL;
    schedInfo->n = 0;
    schedInfo->N = 100; // each 100th period -> 2Hz
    
    // Configure Info of fourth task
    schedInfo++;
    schedInfo->task = &task4_check_excess_current;
    schedInfo->params = NULL;
    schedInfo->n = 0;
    schedInfo->N = 1; // each period -> 200 Hz;

}