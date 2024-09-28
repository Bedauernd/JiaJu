#ifndef __ESP8266_H
#define __ESP8266_H

#include <stdbool.h>

void ESP8266_Init(void);
bool ESP8266_SUB();
bool ESP8266_PUB();
void command(void);

#endif
