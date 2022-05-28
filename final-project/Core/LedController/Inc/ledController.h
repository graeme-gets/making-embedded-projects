/*
 * lecController.h
 *
 *  Created on: 25 May 2022
 *      Author: GraemeGets
 */

#ifndef LEDCONTROLLER_INC_LEDCONTROLLER_H_
#define LEDCONTROLLER_INC_LEDCONTROLLER_H_

#include "main.h"
#define PIXELS_PER_FACE 12
#define PIXEL_RINGS 12

void ledAllOff();
void ledSetFaceColour(uint8_t face, uint8_t red, uint8_t green, uint8_t blue);
void ledDance();
#endif /* LEDCONTROLLER_INC_LEDCONTROLLER_H_ */
