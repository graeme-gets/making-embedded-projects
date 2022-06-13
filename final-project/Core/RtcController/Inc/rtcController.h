/*
 * rtcController.h
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */


#ifndef RTCCONTROLLER_INC_RTCCONTROLLER_H_
#define RTCCONTROLLER_INC_RTCCONTROLLER_H_

#include "time.h"
#include "rtc.h"


typedef enum{
	RTC_OK,
	RTC_INVALID_DATE,
	RTC_INVALID_TIME
}eRTCError_t;

void rtcGetTimeString(char* timeString);
void rtcGetDateString(char* dateString);
time_t rtcGetTimeStamp();
eRTCError_t rtcSetTime(RTC_TimeTypeDef *time);
eRTCError_t rtcSetDate(RTC_DateTypeDef *date);

#endif /* RTCCONTROLLER_INC_RTCCONTROLLER_H_ */
