/*
 * File:   timer.c
 * Author: filip,Matteo
 *
 * Created on October 30, 2020, 8:42 PM
 */


#include "xc.h"

#include "timer.h"
// function to setup a period
void tmr_setup_period(int timer, int ms){
    int tmr_setup_period(int timer, int ms){
    uint16_t sys_fy=18432; // system frequency
    float prescaler_ = 29/1000.0*ms; //prescaler frequency to help calculation
    uint8_t prescaler=0; // prescaler to be set
    uint16_t scaler=0; // prescaler correspondet value
    // Automatic prescaler choice
    if(prescaler_<1){
        prescaler=0;
        scaler=1;
    }else if(prescaler_>=1 && prescaler_<8){
        prescaler=1;
        scaler=8;
    }else if(prescaler_>=8 && prescaler_<64){
        prescaler=2;
        scaler=64;
    }else if(prescaler_>=64 && prescaler_<256){
        prescaler=3;
        scaler=256;
    }else{
        return 1;
    }
    // Timer setup
    switch(timer){
        case 1:
            TMR1=0; // reset the timer
            T1CONbits.TCKPS=prescaler; // set the prescaler
            PR1=sys_fy*(ms/(scaler*10.0)); // set the maximum int to count
            T1CONbits.TON=1; // activate the caunter
            break;
        case 2:
            TMR2=0; // reset the timer
            T2CONbits.TCKPS=prescaler; // set the prescaler
            PR2=sys_fy*(ms/(scaler*10.0)); // set the maximum int to count
            T2CONbits.TON=1; // activate the caunter
            break;
    }
    return 0;
}
}

// function to wait the timer period
void tmr_wait_period(int timer){
    // wait for the timer to expire
    switch(timer){
        case 1:
            while(IFS0bits.T1IF != 1){
            }
            IFS0bits.T1IF = 0; // reset the expire flag to 0
            break;
        case 2:
            while(IFS0bits.T2IF != 1){
            }
            IFS0bits.T2IF = 0; // reset the expire flag to 0
            break;
    }
}

// function to implement a delay
void tmr_wait_ms(int timer,int ms){
    tmr_setup_period(timer,ms); //  Setup the timer for the desired time interval ms 
    switch(timer){ // Cases for each timer
        case 1:
            T1CONbits.TON=1;// start the timer
            while(IFS0bits.T2IF != 1){ // wait the period expiration
            }
            IFS0bits.T1IF = 0; // reset the flag to zero
            T1CONbits.TON=0;//turn of the timer
            break;
        case 2:
            T2CONbits.TON=1;// start the timer
            while(IFS0bits.T2IF != 1){// wait the period expiration
            }
            IFS0bits.T2IF = 0;// reset the flag to zero
            T2CONbits.TON=0;//turn of the timer
            break;
    }
}
