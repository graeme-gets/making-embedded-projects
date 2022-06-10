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



typedef enum
{
	LED_FACE_MODE_NORMAL,
	LED_FACE_MODE_ERROR,
	LED_FACE_MODE_HALF
}eLedFaceMode_t;





void ledAllOff();
void ledSetFaceColour(uint8_t face, uint32_t colour, uint32_t colour2,eLedFaceMode_t mode);
void ledDance();
void ledRender();

#endif /* LEDCONTROLLER_INC_LEDCONTROLLER_H_ */
