/*
 * rtcController.c
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */

#include "rtcController.h"
#include "rtc.h"
#include "stdio.h"
#include "time.h"




void rtcGetTimeString(char* timeString)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); // There is a bug in HAL where the time is only returned if the date is also read (even after the fact!)
	sprintf(timeString,"%02d:%02d:%02d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds);

}

void rtcGetDateString(char* dateString)
{
	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	sprintf(dateString,"%02d-%02d-%02d\t",sDate.Date,sDate.Month,sDate.Year);
}


time_t rtcGetTimeStamp()
{
	time_t timeResult;
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, FORMAT_BIN);

	struct tm tm;

	tm.tm_sec =  time.Seconds;
	tm.tm_min= time.Minutes;
	tm.tm_hour= time.Hours;
	tm.tm_mday= date.Date;
	tm.tm_mon= date.Month - 1;
	tm.tm_year= date.Year + 100;
	tm.tm_isdst= 0;

	timeResult = mktime(&tm);
	return timeResult;

}
