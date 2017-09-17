#include "mq7.h"

void mq7_gpio_init(void)
{
    GPIO_InitTypeDef g;
    
    RCC_APB2PeriphClockCmd(MQ7_RCC, ENABLE);
    
//    // init vcc port
//    g.GPIO_Pin = MQ7_VCC_GPIO_PIN;
//    g.GPIO_Mode = GPIO_Mode_Out_PP;
//    g.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(MQ7_GPIO_TYPE, &g);
//    GPIO_SetBits(MQ7_GPIO_TYPE, MQ7_VCC_GPIO_PIN);
//    
//    // init gnd port
//    g.GPIO_Pin = MQ7_GND_GPIO_PIN;
//    g.GPIO_Mode = GPIO_Mode_Out_PP;
//    g.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(MQ7_GPIO_TYPE, &g);
//    GPIO_ResetBits(MQ7_GPIO_TYPE, MQ7_GND_GPIO_PIN);
    
    // init data port
    g.GPIO_Pin = MQ7_DATA_GPIO_PIN;
    g.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MQ7_GPIO_TYPE, &g);
}

u8 mq7_alarm(void)
{
    if (MQ7_IN == RESET)
    {
        delay_us(CHECK_DELAY_TIME);
        if (MQ7_IN == RESET)
        {
            // alarm
            return 1;
        }
    }
    return 0;
}