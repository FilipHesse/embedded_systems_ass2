/*
 * File:   scheduler.c
 * Author: filip
 *
 * Created on December 9, 2020, 11:32 AM
 */


#include "xc.h"
#include "tasks.h"

void scheduler(SchedInfo * schedInfo) {
    int i;
    for (i = 0; i < MAX_TASKS; i++, schedInfo++) {
        schedInfo->n++;
        if (schedInfo->n >= schedInfo->N) {
            schedInfo->task(schedInfo->params);
            schedInfo->n = 0;
        }
    }
}