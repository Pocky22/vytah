/*
 * ledky.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

/**
* @file ledky.c
* @brief funkcie pre ledky
* @details nasledujuce funkcie riesia zapnutie a vypnutie LED diod
*
*/

#include "ledky.h"
#include "library.h"

void LED(uint8_t led, uint8_t zapnutVypnut) {
	uint8_t crcData[] = {led, 0x00, zapnutVypnut};
	uint8_t msg[] = {0xA0, led, 0x00, 0x01, zapnutVypnut, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
}

void LEDoff(void) {
	LED(0x10,OFF);
	LED(0x11,OFF);
	LED(0x12,OFF);
	LED(0x13,OFF);
	LED(0x14,OFF);

	LED(0x20,OFF);
	LED(0x21,OFF);
	LED(0x22,OFF);
	LED(0x23,OFF);
	LED(0x24,OFF);
}
