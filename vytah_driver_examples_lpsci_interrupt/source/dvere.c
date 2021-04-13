/*
 * dvere.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

#include "dvere.h"
#include "library.h"

void zatvorDvere(void) {
	uint8_t msg[] = {0xA0, 0xF0, 0x00, 0x01, 0x01, 0xD3};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void otvorDvere(void) {
	uint8_t msg[] = {0xA0, 0xF0, 0x00, 0x01, 0x00, 0x8D};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}
