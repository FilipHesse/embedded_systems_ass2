/*
 * File:   tasks.c
 * Author: filip
 *
 * Created on December 9, 2020, 10:17 AM
 */


#include "xc.h"
#include "tasks.h"
#include <stddef.h>
#include "timer.h"

void* task1_control(void* params) {
    // do something for TASK 1
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

void configure_tasks_for_scheduling(SchedInfo * schedInfo){
    // Minimal period duration: 5ms; other periods can be divided by 5 ms
    tmr_setup_period(TIMER1,5); //200 Hz
    
    // Configure Info of first task
    schedInfo->task = &task1_control;
    schedInfo->params = NULL;
    schedInfo->n = 0;
    schedInfo->N = 1; // each period -> 200 Hz
    
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
    schedInfo->N = 200; // each 200th period -> 1Hz
    
}