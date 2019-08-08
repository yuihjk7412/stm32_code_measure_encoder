// TIM—通用-捕获-测量脉宽 应用
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h"  



void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //????
      while(i--) ;    
   }
}
//??????
void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //????
      while(i--) ;    
   }
}

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	uint32_t time;
	int pos1, pos2;
	
	// TIM 计数器的驱动时钟
	uint32_t TIM_PscCLK = 72000000 / (GENERAL_TIM_PSC+1);
	
	/* 串口初始化 */
	USART_Config();
	
	/* 定时器初始化 */
	GENERAL_TIM_Init();
	
	printf ( "\r\n编码器读取实验\r\n" );
	printf ( "\r\n显示当前角度值\r\n" );
	
	while ( 1 )
	{
		//pos1 = TIM_GetCounter(TIM4);
		//pos2 = TIM_GetCounter(TIM3);
		/*Usart_SendByte(USART1, 0x7f);
		Usart_SendHalfWord(USART1, TIM_GetCounter(TIM4));
		Usart_SendHalfWord(USART1, TIM_GetCounter(TIM3));
		Usart_SendByte(USART1, 0xf7);*/
		//printf("\r\nposition1:%d\r\n",pos1/4);
		//printf("\r\nposition2:%d\r\n",pos2/4);
		delay_ms(20);
		
		/*
		if(TIM_ICUserValueStructure.Capture_FinishFlag == 1)
		{
			// 计算高电平时间的计数器的值
			time = TIM_ICUserValueStructure.Capture_Period * (GENERAL_TIM_PERIOD+1) + 
			       (TIM_ICUserValueStructure.Capture_CcrValue+1);
			
			// 打印高电平脉宽时间
			printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
			
			TIM_ICUserValueStructure.Capture_FinishFlag = 0;			
		}		
		*/
	}
}
/*********************************************END OF FILE**********************/
