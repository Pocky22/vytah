
#include "board.h"
#include "fsl_lpsci.h"

#include "clock_config.h"
#include "pin_mux.h"

#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_LPSCI_IRQn UART0_IRQn
#define DEMO_LPSCI_IRQHandler UART0_IRQHandler

#define LP0 0xE0
#define LNP1 0xE1
#define LNP2 0xE2
#define LNP3 0xE3
#define LNP4 0xE4

#define HORE 0x01
#define DOLE 0x02
#define ON 0x01
#define OFF 0x00

_Bool zatvoreneDvere;
uint8_t packet[30], index = 0;
uint8_t data, startovaciBajt, addr, sprava, dataSize, crc;
uint8_t LimitSwitch = 0;
uint8_t poslednaPozicia = 0;
uint8_t poschodie = 0;
uint8_t aktualnePoschodie = 0;


// https://stackoverflow.com/questions/29214301/ios-how-to-calculate-crc-8-dallas-maxim-of-nsdata

unsigned char dallas_crc8(const unsigned char * data, const unsigned int size) {
    unsigned char crc = 0;

    for ( unsigned int i = 0; i < size; ++i ) {
        unsigned char inbyte = data[i];

        for ( unsigned char j = 0; j < 8; ++j ) {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if ( mix ) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

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

//https://cboard.cprogramming.com/c-programming/179195-random-time-delay.html

void delay(int milli_seconds) {
    /// Storing start time
    clock_t start_time = clock();

    /// looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

void posliACK(void) {
	uint8_t crcData[3] = {addr, sprava, 0x00};
	uint8_t newCrc = dallas_crc8(crcData, 3);
	uint8_t msg[] = {0xA1, sprava, addr, 0x00, newCrc};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

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

void pociatok(void) {
	terminalUVOD();
	delay(200);
	zatvorDvere();
	zobrazPoziciu(DOLE);
	zatvoreneDvere = true;
	delay(100);
	if(zatvoreneDvere = true) {
	    pohybDole();
	    terminalDOLE();
	    citajSpravu();
	    while(LimitSwitch != 0xe0) {
	    		citajSpravu();
	    		zobrazPoziciu(DOLE);
	    }
	    delay(10);
	    zastav();
	    terminalSTOP();
	    //citajSpravu();
	    delay(100);
	    otvorDvere();
	    zobrazPoziciu(OFF);
	    poslednaPozicia = LP0;
	    zatvoreneDvere = false;
	    return;
	}
}

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

void terminalSTOP(void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x53, 0x54, 0x4F, 0x50};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x53, 0x54, 0x4F, 0x50, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	citajSpravu();
	delay(50);
}

void terminalP (void) {
	uint8_t crcData[] = {0xd0, 0x00, 0xA, 0x3E, 0x20, 0x50};
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x04, 0xA, 0x3E, 0x20, 0x50, dallas_crc8(crcData, sizeof(crcData))};
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
	uint8_t msg[] = {0xa0, 0xd0, 0x00, 0x05, 0xA, 0x3E, 0x20, 0x50, 0x31, dallas_crc8(crcData, sizeof(crcData))};
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

int main(void) {
	lpsci_config_t config;

	int pom = 1;

		BOARD_InitPins();
	    BOARD_BootClockRUN();
		CLOCK_SetLpsci0Clock(0x1U);

	    LPSCI_GetDefaultConfig(&config);
		config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;

	    LPSCI_Init(DEMO_LPSCI, &config, DEMO_LPSCI_CLK_FREQ);
		LPSCI_EnableTx(DEMO_LPSCI, true);
		LPSCI_EnableRx(DEMO_LPSCI, true);

    while (1) {
    	while(pom == 1) {
    		pociatok();
    		pom = 2;
    	}
    	spracujSpravu();
    }
}
