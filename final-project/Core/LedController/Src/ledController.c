/*
 * ledController.c
 *
 *  Created on: 25 May 2022
 *      Author: GraemeGets
 */

#include "ledController.h"
#include "ws2812.h"
#include "string.h"
#include "colours.h"







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
	HAL_Delay(10);
}

void ledSetFaceColour(uint8_t face, uint32_t colour, uint32_t colour2,eLedFaceMode_t mode)
{
	uint16_t offset = face * 12;



		for (uint16_t i=offset;i<offset + PIXELS_PER_FACE;i++)
		{
			if (mode == LED_FACE_MODE_ERROR)
			{
				uint32_t errorColour = colourFindByid(COLOUR_ERROR_ID)->code;
				if ((i+1)%2)
					led_set_RGB(i,(colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF);
				else
					led_set_RGB(i,(errorColour >> 16) & 0xFF, (errorColour >> 8) & 0xFF, errorColour & 0xFF);
			}
			else if (mode == LED_FACE_MODE_NORMAL)
			{
				led_set_RGB(i,(colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF);
			}
			else if (mode == LED_FACE_MODE_HALF)
			{

				if ((i+1)%2)
					led_set_RGB(i,(colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF);
				else
					led_set_RGB(i,(colour2 >> 16) & 0xFF, (colour2 >> 8) & 0xFF, colour2 & 0xFF);
			}
	}


}


void ledDance()
{
	int i=0;
	while (i<1){
	 uint8_t angle = 0;
	 const uint8_t angle_difference = 11;
	  for(uint8_t i = 0; i < NUM_PIXELS /* Change that to your amount of LEDs */; i++) {
		  // Calculate color
			 HAL_Delay(100);
			uint32_t rgb_color = hsl_to_rgb(angle + (i * angle_difference), 255, 127);
			// Set color
			led_set_RGB(i, (rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF);
			if (i>6)
				led_set_RGB(i-6, 0,0,0);
			++angle;
			led_render();

		}
	  HAL_Delay(10);
		// Write to LED
	  ledAllOff();
	  i++;
	}
}
