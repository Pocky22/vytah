/*
 * pociatok.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

#include "pociatok.h"
#include "library.h"
#include "premenne.h"

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
