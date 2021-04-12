/*
 * premenne.h
 *
 *  Created on: 12. 4. 2021
 *      Author: PC1
 */
#include "library.h"

#ifndef PREMENNE_H_
#define PREMENNE_H_

extern volatile uint8_t packet[30], index;
extern uint8_t data, startovaciBajt, addr, sprava, dataSize, crc;
extern uint8_t LimitSwitch;
extern uint8_t poslednaPozicia;
extern uint8_t poschodie;
extern uint8_t aktualnePoschodie;
extern _Bool zatvoreneDvere;

#endif /* PREMENNE_H_ */
