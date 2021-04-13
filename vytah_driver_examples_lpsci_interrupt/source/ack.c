/*
 * ack.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

#include "ack.h"
#include "library.h"
#include "premenne.h"

void posliACK(void) {
	uint8_t crcData[3] = {addr, sprava, 0x00};
	uint8_t newCrc = dallas_crc8(crcData, 3);
	uint8_t msg[] = {0xA1, sprava, addr, 0x00, newCrc};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

