
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include "ringbuffer.h"

void uart_config(int uartNumber, int baudRate);
ring_buffer_t* getUARTRingBuffer();
void uart2TransmitIntAsStr(int intToSend);

#endif	/* XC_HEADER_TEMPLATE_H */

