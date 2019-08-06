#ifndef __DELAY_H
#define __DELAY_H 			   
#include "core_cm3.h"  
	 
void SysTick_Handler(void);
void delay_ms(volatile unsigned long nms);
void delay_us(volatile unsigned long nus);

#endif
