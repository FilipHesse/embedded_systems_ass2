/*
 * File:   uart.c
 * Author: filip
 *
 * Created on October 30, 2020, 11:00 PM
 */
#include "xc.h"
#include "uart.h"
#include <stdio.h>
#define QUEUE_SIZE 32


//Ring buffer, that contains all the data which has been read
ring_buffer_t uart_ring_buffer;

//Interrupt Service Routine for Receiving from UART1
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(){
    // reset interrupt flag
    IFS0bits.U1RXIF = 0;
    
    // push char into ringbuffer
    ring_buffer_queue(&uart_ring_buffer, U1RXREG);
}


void uart_config(int uartNumber, int baudRate)
{
    if (uartNumber == 1)
    {
        ring_buffer_init(&uart_ring_buffer); //Initialize ring buffer, size: 128 Bytes (defined in rungbuffer.h)
        
        U1BRG = (7372800 / 4) / (16 * baudRate) - 1;
        U1MODEbits.UARTEN = 1;  //Enable UART1
        U1STAbits.UTXEN = 1;
        
        IEC0bits.U1RXIE = 1;    //Enable UART interrupts for receiving
        U1STAbits.URXISEL = 0;  //Interrupt is set, when a character is received
                                // We immediately queue the contents in our 
                                //internal buffer
    }
    
    if (uartNumber == 2)
    {
        U2BRG = (7372800 / 4) / (16 * baudRate) - 1;
        U2MODEbits.UARTEN = 1;
        U2STAbits.UTXEN = 1;
    }
}

// Getter function for ring buffer struct
ring_buffer_t* getUARTRingBuffer()
{
    return &uart_ring_buffer;
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