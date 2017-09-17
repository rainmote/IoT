#ifndef __MQ7_H_
#define __MQ7_H_

#include "stm32f10x_gpio.h"
#include "timer.h"

// 用户自定义接口部分
// vcc -> PB12
// GND -> PB13
// DO  -> PB14
#define MQ7_RCC             RCC_APB2Periph_GPIOB
#define MQ7_GPIO_TYPE       GPIOB
#define MQ7_VCC_GPIO_PIN    GPIO_Pin_12
#define MQ7_GND_GPIO_PIN    GPIO_Pin_13
#define MQ7_DATA_GPIO_PIN   GPIO_Pin_9

#define MQ7_IN  GPIO_ReadInputDataBit(MQ7_GPIO_TYPE, MQ7_DATA_GPIO_PIN)

// 检测防干扰延时
#define CHECK_DELAY_TIME 5000000

void mq7_gpio_init(void);
u8 mq7_alarm(void);

#endif