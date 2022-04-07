/*
 * ledController.h
 *
 *  Created on: Apr 6, 2022
 *      Author: GraemeGets
 */

#ifndef INC_LEDCONTROLLER_H_
#define INC_LEDCONTROLLER_H_


void ledToggle();
void ledOff(uint8_t force);
void ledOn();
uint8_t ledControlStateToggle();
void setLedControlState(uint8_t state);

#endif /* INC_LEDCONTROLLER_H_ */
