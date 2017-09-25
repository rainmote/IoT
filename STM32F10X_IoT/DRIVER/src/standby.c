#include "standby.h"
#include "usart.h"

void rtc_configuration(void)
{
    NVIC_InitTypeDef n;
    EXTI_InitTypeDef e;
    
    //中断时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    //中断优先级配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    //设置RTC闹钟中断
    n.NVIC_IRQChannel = RTCAlarm_IRQn;
    n.NVIC_IRQChannelPreemptionPriority = 0;
    n.NVIC_IRQChannelSubPriority = 3;
    n.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&n);
    
    //闹钟中断接到第17线外部中断
    EXTI_ClearITPendingBit(EXTI_Line17);
    e.EXTI_Line = EXTI_Line17;
    e.EXTI_Mode = EXTI_Mode_Interrupt;
    e.EXTI_Trigger = EXTI_Trigger_Rising;
    e.EXTI_LineCmd = ENABLE;
    EXTI_Init(&e);
    
    //Enable PWR and BKP clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    //使能后备寄存器访问
    PWR_BackupAccessCmd(ENABLE);
    BKP_ClearFlag();
    BKP_DeInit();
    
    //使能LSI
    RCC_LSICmd(ENABLE);
    
    //等待晶振启动
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    
    //设置时钟为内部晶振
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    
    //等待RTC_CTL寄存器中的RSF位（寄存器同步标志）被硬件置为1
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    
    //使能闹钟中断
    RTC_ITConfig(RTC_IT_ALR, ENABLE);
    RTC_WaitForLastTask();
    
    //分频系数
    //RTC period = RTCCLK / RTC_PR = (32.768 KHz) / (32767 + 1)
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();
    
    //初始化计数值
    RTC_SetCounter(0);
    RTC_WaitForLastTask();
    
    //设置闹铃时钟
    RTC_SetAlarm(RTC_GetCounter() + 30);
    RTC_WaitForLastTask();
}

void RTCAlarm_IRQHandler(void)
{
    //等待RTC_CTL寄存器中的RSF位（寄存器同步标志）被硬件置1
    RTC_WaitForSynchro();
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        usart_printf("RTC sec2...");
        //清除EXTI_Line17挂起位
        EXTI_ClearITPendingBit(EXTI_Line17);
        
        //检查唤醒标志是否设置
        if (PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
        {
            //清除唤醒标志
            PWR_ClearFlag(PWR_FLAG_WU);
        }
        
        //Clear the RTC Second interrupt
        RTC_SetCounter(0);
        RTC_WaitForLastTask();
        RTC_ClearITPendingBit(RTC_IT_ALR);
        RTC_WaitForLastTask();    
    }
}

void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        usart_printf("RTC sec...");
        RTC_ClearITPendingBit(RTC_IT_SEC);
        RTC_WaitForLastTask();    
    }
}