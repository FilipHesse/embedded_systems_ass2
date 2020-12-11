/*
 * File:   uart.c
 * Author: filip,Matteo
 *
 * Created on October 30, 2020, 11:00 PM
 */
#include "xc.h"
#include "uart.h"
#include <stdio.h>
#define QUEUE_SIZE 32


//Ring buffer, that contains all the data which has been read
volatile ring_buffer_t uart_ring_buffer;

//Interrupt Service Routine for Receiving from UART1
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(){
    // reset interrupt flag
    IFS0bits.U1RXIF = 0;
    
    // push char into ringbuffer
    ring_buffer_queue(&uart_ring_buffer, U1RXREG);
}


void uart_config(int uartNumber, int baudRate)
{
    // Configure UART1 for receiving into the ring buffer
    if (uartNumber == 1)
    {
        ring_buffer_init(&uart_ring_buffer); //Initialize ring buffer, size: 128 Bytes (defined in rungbuffer.h)
        
        //U1BRG = (7372800 / 4) / (16 * baudRate) - 1; => overflow
        //      = 115200/baudRate - 1                  => still overflow
        //      = (57600/baudRate)*2- 1                => no overflow
        
        U1BRG = (57600/baudRate)*2- 1;  // works for standard baud rates 150, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600
        U1MODEbits.UARTEN = 1;  //Enable UART1
        U1STAbits.UTXEN = 1;
        
        IEC0bits.U1RXIE = 1;    //Enable UART interrupts for receiving
        U1STAbits.URXISEL = 0;  //Interrupt is set, when a character is received
                                // We immediately queue the contents in our 
                                //internal buffer
    }
    
    
    if (uartNumber == 2)
    {
        U2BRG = (57600/baudRate)*2- 1;
        U2MODEbits.UARTEN = 1;
        U2STAbits.UTXEN = 1;
    }
}

// Get safely character fromUART buffer
// if empty, returns 0
// if character available, then one character will be written into data and return value = 1
uint8_t getUart1CharFromBuffer(char *data){
    uint8_t ret;
    // Deactivate ISR for the one function call to avoid conflicts with ISR:
    U1MODEbits.UARTEN = 0;
    ret = ring_buffer_dequeue(&uart_ring_buffer, data);
    // Activate ISR again
    U1MODEbits.UARTEN = 1;
    return ret;    
}

void uart2TransmitIntAsStr(int intToSend)
{
    // Convert counter number to string
    char counterStr[10] = "";
    sprintf(counterStr, "%u", intToSend);

    // iterate over the characters of the string and send all of them
    int i = 0;
    while(counterStr[i] != '\0')
    {
        //Send counter number to UART
        U2TXREG = counterStr[i];
        i=i+1;
    }
}

// function to send a character array over uart2
void uart2TransmitStr(char * string)
{
    
    // iterate over the characters of the string and send all of them
    int i = 0;
    while(string[i] != '\0')
    {
        //Send counter number to UART
        U2TXREG = string[i];
        i=i+1;
    }
}