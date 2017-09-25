#include "standby.h"
#include "usart.h"

void rtc_configuration(void)
{
    NVIC_InitTypeDef n;
    EXTI_InitTypeDef e;
    
    //�ж�ʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    //�ж����ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    //����RTC�����ж�
    n.NVIC_IRQChannel = RTCAlarm_IRQn;
    n.NVIC_IRQChannelPreemptionPriority = 0;
    n.NVIC_IRQChannelSubPriority = 3;
    n.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&n);
    
    //�����жϽӵ���17���ⲿ�ж�
    EXTI_ClearITPendingBit(EXTI_Line17);
    e.EXTI_Line = EXTI_Line17;
    e.EXTI_Mode = EXTI_Mode_Interrupt;
    e.EXTI_Trigger = EXTI_Trigger_Rising;
    e.EXTI_LineCmd = ENABLE;
    EXTI_Init(&e);
    
    //Enable PWR and BKP clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    //ʹ�ܺ󱸼Ĵ�������
    PWR_BackupAccessCmd(ENABLE);
    BKP_ClearFlag();
    BKP_DeInit();
    
    //ʹ��LSI
    RCC_LSICmd(ENABLE);
    
    //�ȴ���������
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    
    //����ʱ��Ϊ�ڲ�����
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    
    //�ȴ�RTC_CTL�Ĵ����е�RSFλ���Ĵ���ͬ����־����Ӳ����Ϊ1
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    
    //ʹ�������ж�
    RTC_ITConfig(RTC_IT_ALR, ENABLE);
    RTC_WaitForLastTask();
    
    //��Ƶϵ��
    //RTC period = RTCCLK / RTC_PR = (32.768 KHz) / (32767 + 1)
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();
    
    //��ʼ������ֵ
    RTC_SetCounter(0);
    RTC_WaitForLastTask();
    
    //��������ʱ��
    RTC_SetAlarm(RTC_GetCounter() + 30);
    RTC_WaitForLastTask();
}

void RTCAlarm_IRQHandler(void)
{
    //�ȴ�RTC_CTL�Ĵ����е�RSFλ���Ĵ���ͬ����־����Ӳ����1
    RTC_WaitForSynchro();
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        usart_printf("RTC sec2...");
        //���EXTI_Line17����λ
        EXTI_ClearITPendingBit(EXTI_Line17);
        
        //��黽�ѱ�־�Ƿ�����
        if (PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
        {
            //������ѱ�־
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