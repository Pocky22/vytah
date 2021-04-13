/*
 * zobrazPoziciu.c
 *
 *  Created on: 12. 4. 2021
 *      Author: PC1
 */

/**
* @file zobrazPoziciu.c
* @brief funkcia pre zobrazovanie inforamcii na displej
* @details funkcia, ktora zobrazuje na displej poschodie kde sa vytah nachadza a smer, ktorym sa pohybuje
*
*/

#include "zobrazPoziciu.h"
#include "library.h"
#include "premenne.h"

void zobrazPoziciu(uint8_t smer) {
	switch (sprava) {
		case 0xE0:
			aktualnePoschodie = 0x50;
			break;
		case 0xE1:
			aktualnePoschodie = 0x31;
			break;
		case 0xE2:
			aktualnePoschodie = 0x32;
			break;
		case 0xE3:
			aktualnePoschodie = 0x33;
			break;
		case 0xE4:
			aktualnePoschodie = 0x34;
			break;
	}
	uint8_t crcData[] = {0x30, 0x00, smer, aktualnePoschodie};
	uint8_t msg[] = {0xA0, 0x30, 0x00, 0x02, smer, aktualnePoschodie, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
}
