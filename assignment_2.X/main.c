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


int main() {
    //InitDevices();
    // ...
    
    SchedInfo schedInfo[MAX_TASKS];
    configure_tasks_for_scheduling(&schedInfo[0]);
    
    while (1) {
        scheduler(&schedInfo[0]);
        tmr_wait_period(TIMER1);
    }
}


