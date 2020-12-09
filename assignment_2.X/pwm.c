/*
 * File:   pwm.c
 * Author: filip
 *
 * Created on December 9, 2020, 4:34 PM
 */


#include "xc.h"

void setup_pwm_H2()
{
    PTCONbits.PTMOD = 0;    //Free running
    PWMCON1bits.PEN2H = 1;    
    
    //Specify Frequency of 50 Hz: PTPER = Fcy / ( Prescaler * PWM_frequency ) - 1
    //Needs to fit into 15 bit value! Prescaler 1, does not work => use 4
    // 1843200 / (4*50Hz) - 1 = 9215
    PTCONbits.PTCKPS = 1;   //Prescaler = 4
    PTPER = 9215;   
    
    PTCONbits.PTEN = 1;     //Enable PWM time base                   
}

void set_percentage_value_pwm_H2(double duty_perc)
{
    PDC2 = 2 * PTPER * duty_perc;
}
