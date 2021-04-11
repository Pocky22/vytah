
#include "board.h"
#include "fsl_lpsci.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "LPSCI functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";
_Bool zatvoreneDvere, readed;
uint8_t packet[20], index = 0;
uint8_t data, startovaciBajt, adr1, sprava, dataSize, crc;
uint8_t LimitSwitch = 0;
uint8_t poslednaPozicia = 0;
uint8_t poschodie = 0;

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_LPSCI_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kLPSCI_RxDataRegFullFlag)&LPSCI_GetStatusFlags(DEMO_LPSCI))
    {
        data = LPSCI_ReadByte(DEMO_LPSCI);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
            demoRingBuffer[rxIndex] = data;
            rxIndex++;
            rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}

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
		adr1 = data;

		if(!(adr1 != 0x00)) break;
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
			poschodie = LP0;
		} else if(sprava == 0xC1 || sprava == 0xB1) {
			poschodie = LNP1;
		} else if(sprava == 0xC2 || sprava == 0xB2) {
			poschodie = LNP2;
		} else if(sprava == 0xC3 || sprava == 0xB3) {
			poschodie = LNP3;
		} else if(sprava == 0xc4 || sprava == 0xB4) {
			poschodie = LNP4;
		}
		if(poslednaPozicia < poschodie) {
			zatvorDvere();
			delay(100);
			pohybHore();
			while(LimitSwitch != poschodie) {
				citajSpravu();
			}
			delay(10);
			zastav();
			citajSpravu();
			delay(100);
			otvorDvere();
			poslednaPozicia = LimitSwitch;

		} else if(poslednaPozicia > poschodie){
			zatvorDvere();
			delay(100);
			pohybDole();
			while(LimitSwitch != poschodie) {
				citajSpravu();
			}
			delay(10);
			zastav();
			citajSpravu();
			delay(100);
			otvorDvere();
			poslednaPozicia = LimitSwitch;
		}

}

//https://cboard.cprogramming.com/c-programming/179195-random-time-delay.html

void delay(int milli_seconds)
{
    /// Storing start time
    clock_t start_time = clock();

    /// looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

void posliACK(void) {
	uint8_t crcData[3] = {adr1, sprava, 0x00};
	uint8_t newCrc = dallas_crc8(crcData, 3);
	uint8_t msg[] = {0xA1, sprava, adr1, 0x00, newCrc};
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
	zatvorDvere();
	zatvoreneDvere = true;
	delay(100);
	if(zatvoreneDvere = true) {
	    pohybDole();
	    citajSpravu();
	    while(LimitSwitch != 0xe0) {
	    		citajSpravu();

	    }
	    delay(10);
	    zastav();
	    citajSpravu();
	    delay(100);
	    otvorDvere();
	    poslednaPozicia = LP0;
	    zatvoreneDvere = false;
	    return;
	}
}
/*!
 * @brief Main function
 */
int main(void)
{
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
