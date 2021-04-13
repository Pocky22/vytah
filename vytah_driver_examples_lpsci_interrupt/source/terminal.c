/*
 * terminal.c
 *
 *  Created on: 12. 4. 2021
 *      Author: PC1
 */

/**
* @file terminal.c
* @brief zobrazovanie informacnych udajov
* @details nasledujuce funkcie maju za ulohu informovat pomocou terminalu o smere pohybu, ktory vykonava vytah a zobrazovat ake poschodie bolo stlacene
*
*/

#include "terminal.h"
#include "library.h"

void terminalSTOP(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x53, 0x54, 0x4F, 0x50};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x53, 0x54, 0x4F, 0x50, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminalP (void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x3E, 0x20, 0x50, 0x30};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x3E, 0x20, 0x50, 0x30, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminal1(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x3E, 0x20, 0x50, 0x31};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x3E, 0x20, 0x50, 0x31, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminal2(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x3E, 0x20, 0x50, 0x32};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x3E, 0x20, 0x50, 0x32, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminal3(void) {
	uint8_t crcData[] = {0xd0, 0x00,  0xA, 0x3E, 0x20, 0x50, 0x33};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x3E, 0x20, 0x50, 0x33, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminal4(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x3E, 0x20, 0x50, 0x34};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x3E, 0x20, 0x50, 0x34, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminalHORE(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x2D, 0x20, 0x48, 0x4F, 0x52, 0x45};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x07, 0xA, 0x2D, 0x20, 0x48, 0x4F, 0x52, 0x45, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminalDOLE(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x2D, 0x20, 0x44, 0x4F, 0x4C, 0x45};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x07, 0xA, 0x2D, 0x20, 0x44, 0x4F, 0x4C, 0x45, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminalUVOD(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0x2A, 0x20, 0x56, 0x59, 0x54, 0x41, 0x48, 0x20, 0x2A};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x09, 0x2A, 0x20, 0x56, 0x59, 0x54, 0x41, 0x48, 0x20, 0x2A, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}
