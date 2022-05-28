#ifndef _LED_DRIVER_SK6812
#define _LED_DRIVER_SK6812

#include <stdint.h>
#include "tim.h"

#define LED_TIMER htim1
#define LED_DMA hdma_tim1_ch1
#define LED_DMA_CHANNEL TIM_CHANNEL_1
#define NUM_PIXELS (144)
#define LED_BRIGHTNESS 30 // 50%

void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void led_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
void led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void led_render();
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l);

#endif
