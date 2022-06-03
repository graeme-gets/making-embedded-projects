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

typedef struct
{
	char name[10];
	uint32_t code;

}ledColour_t;

typedef struct
{
	ledColour_t colour[PIXEL_RINGS];
}ledColours_t;







void ledAllOff();
void ledSetFaceColour(uint8_t face, uint8_t red, uint8_t green, uint8_t blue);
void ledDance();
void ledRender();
void ledFindColour(uint32_t code, char* name);
#endif /* LEDCONTROLLER_INC_LEDCONTROLLER_H_ */
