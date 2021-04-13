/*
 * delay.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

/**
* @file delay.c
* @brief funkcia delay
* @details funkcia, ktora caka zadany pocet milisekund
*
*/

#include "delay.h"
#include "clock_config.h"

/*!
* zdroj: //https://cboard.cprogramming.com/c-programming/179195-random-time-delay.html
*/


void delay(int milli_seconds) {
    /// Storing start time
    clock_t start_time = clock();

    /// looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}
