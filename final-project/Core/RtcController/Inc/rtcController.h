/*
 * rtcController.h
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */


#ifndef RTCCONTROLLER_INC_RTCCONTROLLER_H_
#define RTCCONTROLLER_INC_RTCCONTROLLER_H_

#include "time.h"

void rtcGetTimeString(char* timeString);
void rtcGetDateString(char* dateString);
time_t rtcGetTimeStamp();

#endif /* RTCCONTROLLER_INC_RTCCONTROLLER_H_ */
