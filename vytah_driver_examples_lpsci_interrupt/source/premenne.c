/*
 * premenne.c
 *
 *  Created on: 12. 4. 2021
 *      Author: PC1
 */

#include "premenne.h"
#include "library.h"

volatile uint8_t packet[30], index = 0;
uint8_t data, startovaciBajt, addr, sprava, dataSize, crc;
uint8_t LimitSwitch = 0;
uint8_t poslednaPozicia = 0;
uint8_t poschodie = 0;
uint8_t aktualnePoschodie = 0;
_Bool zatvoreneDvere;
