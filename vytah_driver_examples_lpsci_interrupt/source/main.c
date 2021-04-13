#include "library.h"
#include "delay.h"
#include "dvere.h"
#include "motor.h"
#include "terminal.h"
#include "premenne.h"
#include "ledky.h"
#include "crc.h"
#include "ack.h"
#include "pociatok.h"

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
