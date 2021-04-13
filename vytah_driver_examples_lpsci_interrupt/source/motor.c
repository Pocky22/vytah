/*
 * motor.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */


/**
* @file motor.c
* @brief funkcie pre ovladanie pohybu vytahu
* @details nasledujuce funkcie riadia ovladanie pohybu vytahu ako pohyb hore, dole a zastavenie vytahu
*
*/

#include "motor.h"
#include "library.h"

void pohybDole(void) {
	uint8_t msg[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0x9c, 0xff, 0xff, 0xff, 0x6f};	// DOLE (-100)
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void pohybHore(void) {
	uint8_t msg[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0x64, 0x00, 0x00, 0x00, 0x64}; 	// HORE (100)
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void zastav(void) {
	uint8_t msg2[] = {0xa0, 0xf1, 0x00, 0x01, 0x01, 0x78};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg2, sizeof(msg2));
	citajSpravu();
	delay(50);
}
