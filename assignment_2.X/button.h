// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUTTON_H
#define	BUTTON_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include <stdbool.h>

#define BUTTON1
#define BUTTON2
#define BUTTON3

void setupButtonS5();
bool wasButtonS5Pressed();

void setupButtonS6();
bool wasButtonS6Pressed();

#endif	/* XC_HEADER_TEMPLATE_H */

