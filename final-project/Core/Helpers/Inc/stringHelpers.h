/*
 * stringHelpers.h
 *
 *  Created on: 27 Apr 2022
 *      Author: GraemeGets
 */

#ifndef HELPERS_INC_STRINGHELPERS_H_
#define HELPERS_INC_STRINGHELPERS_H_

#define DEL  '003'
#define BKSP  '\177'




void manageBackSpace(char* string);
uint8_t isNumber(uint8_t *string);
void byteToBin(uint8_t b, char*text);
#endif /* HELPERS_INC_STRINGHELPERS_H_ */
