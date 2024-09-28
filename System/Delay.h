#ifndef __DELAY_H
#define __DELAY_H

#include <stm32f10x.h>
#include <stdint.h>

void Delay_us(unsigned int us);
void Delay_ms(unsigned int ms);
void Delay_s(unsigned int s);

#endif
