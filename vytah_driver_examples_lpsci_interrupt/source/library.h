/*
 * library.h
 *
 *  Created on: 12. 4. 2021
 *      Author: PC1
 */

#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "board.h"
#include "fsl_lpsci.h"
#include "pin_mux.h"

#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_LPSCI_IRQn UART0_IRQn
#define DEMO_LPSCI_IRQHandler UART0_IRQHandler
#define ON 0x01
#define OFF 0x00
#define LP0 0xE0
#define LNP1 0xE1
#define LNP2 0xE2
#define LNP3 0xE3
#define LNP4 0xE4

#define HORE 0x01
#define DOLE 0x02

#endif /* LIBRARY_H_ */
