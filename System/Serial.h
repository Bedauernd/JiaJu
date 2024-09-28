#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#include <stdint.h>

void Serial_Init(void);
void Serial_SendByte(unsigned char Byte);
void Serial_SendArray(unsigned char *Array, unsigned short int Length);
void Serial_SendString(char *String);
void Serial_SendNumber(unsigned int Number, unsigned char Length);
void Serial_Printf(char *format, ...);

unsigned char Serial_GetRxFlag(void);
unsigned char Serial_GetRxData(void);

#endif
