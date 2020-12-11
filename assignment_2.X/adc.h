/*
 * File:   adc.h
 * Author:  Matteo
 *
 * Created on December 9, 2020, 10:17 AM
 */

/// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Function to setup the ADC to read temperature and voltage on ch0 and ch1 with
// automatic sampling and converting start
void adcsetup_temp_volt();

#endif	/* XC_HEADER_TEMPLATE_H */
