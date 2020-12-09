/*
 * File:   tasks.c
 * Author: filip
 *
 * Created on December 9, 2020, 10:17 AM
 */


#include "xc.h"

#include <stddef.h>
#include <string.h>
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
    // do something for TASK 2
    return NULL;
}

void* task3_blink(void* params) {
    // do something for TASK 3
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
    schedInfo->n = 2;
    schedInfo->N = 200; // each 200th period -> 1Hz

    // Configure Info of third task
    schedInfo++;
    schedInfo->task = &task3_blink;
    schedInfo->params = NULL;
    schedInfo->n = 4;
    schedInfo->N = 200; // each 200th period -> 1Hz

}