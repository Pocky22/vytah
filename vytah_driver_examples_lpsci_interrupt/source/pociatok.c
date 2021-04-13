/*
 * pociatok.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

/**
* @file pociatok.c
* @brief uvodna funkcia
* @details funkcia, ktora sa vykona automaticky po inicializacii
*
*/

#include "pociatok.h"
#include "library.h"
#include "premenne.h"

/*!
* funkcia, ktora ma za ulohu priviest vytah na pociatocnu poziciu, ktorou je poschodie P. Vytah pustime smerom dole a
* za pomoci limit switchov rozpozname kde sa vytah akutalne nachadza a zastavime ho na poschodi P. Nasledne sa otvoria
* dvere a cakame na privolanie vytahu z urciteho poschodia.
*/

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
