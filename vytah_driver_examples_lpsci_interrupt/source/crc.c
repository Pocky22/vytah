/*
 * crc.c
 *
 *  Created on: 13. 4. 2021
 *      Author: PC1
 */

#include "crc.h"
#include "library.h"

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
