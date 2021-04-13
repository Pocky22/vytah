/*
 * spravy.c
 *
 *  Created on: 12. 4. 2021
 *      Author: PC1
 */

#include "spravy.h"
#include "library.h"
#include "premenne.h"

void citajSpravu(void) {
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	startovaciBajt = data;

	while(true) {
		LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
		addr = data;

		if(!(addr != 0x00)) break;
	}
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	sprava = data;
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	dataSize = data;
	for (int x = 0; x < dataSize; x++) {
			LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
			packet[x] = data;
	}
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	crc = data;
	if (sprava == LP0 || sprava == LNP1 || sprava == LNP2 || sprava == LNP3 || sprava == LNP4) {
		LimitSwitch = sprava;
	}
	if (startovaciBajt == 0xA0) {
		posliACK();
	}
}

void spracujSpravu(void) {
	citajSpravu();
		if (sprava == 0xC0 || sprava == 0xB0) {
			if(sprava == 0xC0) {
				LED(0x10,ON);
				terminalP();
			} else {
				LED(0x20,ON);
				terminalP();
			}
			poschodie = LP0;
		} else if(sprava == 0xC1 || sprava == 0xB1) {
			if(sprava == 0xC1) {
				LED(0x11,ON);
				terminal1();
			} else {
				LED(0x21,ON);
				terminal1();
			}
			poschodie = LNP1;
		} else if(sprava == 0xC2 || sprava == 0xB2) {
			if(sprava == 0xC2) {
				LED(0x12,ON);
				terminal2();
			} else {
				LED(0x22,ON);
				terminal2();
			}
			poschodie = LNP2;
		} else if(sprava == 0xC3 || sprava == 0xB3) {
			if(sprava == 0xC3) {
				LED(0x13,ON);
				terminal3();
			} else {
				LED(0x23,ON);
				terminal3();
			}
			poschodie = LNP3;
		} else if(sprava == 0xc4 || sprava == 0xB4) {
			if(sprava == 0xC4) {
				LED(0x14,ON);
				terminal4();
			} else {
				LED(0x24,ON);
				terminal4();
			}
			poschodie = LNP4;
		}
		if(poslednaPozicia < poschodie) {
			zatvorDvere();
			zobrazPoziciu(HORE);
			delay(100);
			pohybHore();
			terminalHORE();

			while(LimitSwitch != poschodie) {
				citajSpravu();
				zobrazPoziciu(HORE);
			}
			delay(10);
			zastav();
			terminalSTOP();
			LEDoff();
			//citajSpravu();
			delay(100);
			otvorDvere();
			zobrazPoziciu(OFF);
			poslednaPozicia = LimitSwitch;

		} else if(poslednaPozicia > poschodie){
			zatvorDvere();
			zobrazPoziciu(DOLE);
			delay(100);
			pohybDole();
			terminalDOLE();

			while(LimitSwitch != poschodie) {
				citajSpravu();
				zobrazPoziciu(DOLE);
			}
			delay(10);
			zastav();
			terminalSTOP();
			LEDoff();
			//citajSpravu();
			delay(100);
			otvorDvere();
			zobrazPoziciu(OFF);
			poslednaPozicia = LimitSwitch;
		}
}
