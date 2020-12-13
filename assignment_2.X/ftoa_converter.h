// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FTOA_CONVERTER_H
#define	FTOA_CONVERTER_H

#include <xc.h> // include processor files - each processor file is guarded.  

//Function to convert float to string 
char * ftoa(double f, char * buf, int precision);

#endif	/* XC_HEADER_TEMPLATE_H */