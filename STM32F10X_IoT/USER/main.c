#include "stm32f10x.h"
#include "timer.h"
#include "usart.h"

#include "dht11.h"
#include "mq7.h"

#define Led_On   GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define Led_Off  GPIO_ResetBits(GPIOC, GPIO_Pin_13)


void LED_Init()
{
    GPIO_InitTypeDef s;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    s.GPIO_Pin = GPIO_Pin_13;
    s.GPIO_Mode = GPIO_Mode_Out_PP;
    s.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &s);
}

int main()
{
    SystemInit();
    systick_init();
    
    //初始化usart
    usart_config();
    
    //初始化mq7
    mq7_gpio_init();
    
    LED_Init();
    
    while (1)
    {
        Led_On;
         
        u8 buffer[5];
        double humidness;
        double temperature;
        if (dht11_read_data(buffer) == 0)
        {
            humidness = buffer[0] + buffer[1] / 10.0;
            temperature = buffer[2] + buffer[3] / 10.0;
        }
        
        u8 is_alarm = MQ7_IN;
        
        usart_printf("温度: %.2f\t湿度: %.2f\tMQ7:%d\n", temperature, humidness, is_alarm);
       
//        USART_SendData(USART1, buffer[0]);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//        USART_SendData(USART1, buffer[1]);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//        USART_SendData(USART1, buffer[2]);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//        USART_SendData(USART1, buffer[3]);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        
        delay_us(500000);
        Led_Off;
        delay_us(1000000);
    }
}
