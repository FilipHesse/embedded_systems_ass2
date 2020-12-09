/*
 * File:   main.c
 * Author: filip
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


int main() {
    //Setup UART
    uart_config(1, 9600);
    
    //SetupPWM
    setup_pwm_H2();
    
    //Setup scheduling
    SchedInfo schedInfo[MAX_TASKS];
    configure_tasks_for_scheduling(&schedInfo[0]);
    
    while (1) {
        scheduler(&schedInfo[0]);
        tmr_wait_period(TIMER1);
    }
}


