/*
 * File:   button.c
 * Author: Matteo
 *
 * Created on October 30, 2020, 9:08 PM
 */


#include "xc.h"
#include "adc.h"




// Function to setup the ADC to read both temperature and potentiometer voltage
void adcsetup_temp_volt(){
    ADCON3bits.ADCS=8; //Tad
    // Automatic sampling and conversion start
    ADCON1bits.ASAM=1; 
    ADCON1bits.SSRC=7;
    ADCON3bits.SAMC=1;// 1 Tad
    // Channel 0&1
    ADCON2bits.CHPS=1;
    //AN2 on channel 0 with respect to ground
    ADCHSbits.CH0SA=2; // positive pin
    ADCHSbits.CH0NA=0; // with respect to ground
    //AN2 on channel 1 with respect to ground
    ADCHSbits.CH123SA=1; // positive pin
    ADCHSbits.CH123NA=0; // with respect to ground
    // AN2 & AN3 as analog
    ADPCFG=0xFFFF;
    ADPCFGbits.PCFG2=0;
    ADPCFGbits.PCFG2=0;
    //Turn on
    ADCON1bits.ADON=1;
}
