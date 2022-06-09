/*
 * colours.h
 *
 *  Created on: 6 Jun 2022
 *      Author: GraemeGets
 */

#ifndef INC_COLOURS_H_
#define INC_COLOURS_H_
#include <stm32f4xx.h>

#define COLOUR_COUNT_MAX 14
#define COLOUR_NAME_MAX 15

#define COLOUR_RED_ID  	0
#define COLOUR_BLUE_ID		1
#define COLOUR_GREEN_ID	2
#define COLOUR_PURPLE_ID	3
#define COLOUR_ORANGE_ID	4
#define COLOUR_YELLOW_ID	5
#define COLOUR_BROWN_ID	6
#define COLOUR_TEAL_ID		7
#define COLOUR_PEACH_ID	8
#define COLOUR_WHITE_ID	9
#define COLOUR_SKY_ID		10
#define COLOUR_BRICK_ID	11
#define COLOUR_BLACK_ID 12
#define COLOUR_ERROR_ID 13

typedef struct
{
	uint8_t id;
	char name[COLOUR_NAME_MAX];
	uint32_t code;

}colour_t;


colour_t * colourFindByCode(const uint32_t code);
colour_t * colourFindByName(const char * name );
colour_t *colourFindByid(const uint8_t id );

#endif /* INC_COLOURS_H_ */
