/*
 * ledController.c
 *
 *  Created on: 25 May 2022
 *      Author: GraemeGets
 */

#include "ledController.h"
#include "ws2812.h"
#include "string.h"







void ledAllOff()
{
	for (uint8_t i=0;i<NUM_PIXELS;i++)
	{
		led_set_RGB(i,0x0,0,0);
	}

}

void ledRender()
{
	led_render();
}

void ledSetFaceColour(uint8_t face, uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t offset = face * 12;

	for (uint8_t i=offset;i<offset + PIXELS_PER_FACE;i++)
	{
		led_set_RGB(i,red,green,blue);
	}

}


void ledDance()
{
	int i=0;
	while (i<10){
	 uint8_t angle = 0;
	 const uint8_t angle_difference = 11;
	  for(uint8_t i = 0; i < NUM_PIXELS /* Change that to your amount of LEDs */; i++) {
		  // Calculate color
			 HAL_Delay(100);
			uint32_t rgb_color = hsl_to_rgb(angle + (i * angle_difference), 255, 127);
			// Set color
			led_set_RGB(i, (rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF);
			if (i>1)
				led_set_RGB(i-1, 0,0,0);
			++angle;
			led_render();

		}
	  HAL_Delay(10);
		// Write to LED
	  ledAllOff();
	  i++;
	}
}
