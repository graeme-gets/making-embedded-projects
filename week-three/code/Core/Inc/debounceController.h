/*
 * debounceController.h
 *
 *  Created on: Apr 6, 2022
 *      Author: GraemeGets
 */


#ifndef _debounceController_H
#define _debounceController_H


#define msUnits ((1<<11)-1)  // Used to test if the debounce value up to the the bit value is 0 - Cannot be greater than 31  (32 bit number)

uint8_t buttonPressed();
void buttonReset();

#endif
