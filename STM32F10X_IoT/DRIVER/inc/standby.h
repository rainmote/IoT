#ifndef __STANDBY_H__
#define __STANDBY_H__

#include "stm32f10x.h"

void rtc_configuration(void);
void RTCAlarm_IRQHandler(void);
#endif