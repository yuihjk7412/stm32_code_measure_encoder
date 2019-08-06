
#include "bsp_GeneralTim.h"

// 定时器输入捕获用户自定义变量结构体定义
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源为TIM4
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	// 设置中断来源为TIM3
	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // TIM4输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//初始化GPIOB
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;//选定PB6,PB7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选定速率
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	//TIM3输入捕获通道GPIO初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//初始化GPIOA
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

/*--------------------时基结构体初始化-------------------------*/	
	TIM_DeInit(TIM4);
  TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM4_TimeBaseStructure);
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM4_TimeBaseStructure.TIM_Period=2500*4-1;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM4_TimeBaseStructure.TIM_Prescaler= 0;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM4_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM4_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM4_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM4, &TIM4_TimeBaseStructure);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM3_TimeBaseStructure);
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM3_TimeBaseStructure.TIM_Period=2500*4-1;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM3_TimeBaseStructure.TIM_Prescaler= 0;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM3_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM3_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM3_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitTypeDef TIM4_ICInitStructure;
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//???????3,???????
  TIM_ICStructInit(&TIM4_ICInitStructure);//将结构体中的内容缺省输入
  TIM4_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);//将TIM4_ICInitStructure中的指定参数初始化为TIM4
	
	TIM_ICInitTypeDef TIM3_ICInitStructure;
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//???????3,???????
  TIM_ICStructInit(&TIM3_ICInitStructure);//将结构体中的内容缺省输入
  TIM3_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);//将TIM3_ICInitStructure中的指定参数初始化为TIM3
	
	// 清除更新中断标志位
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  // 开启更新中断  
	TIM_ITConfig (TIM4, TIM_IT_Update, ENABLE);
	TIM_ITConfig (TIM3, TIM_IT_Update, ENABLE);
	
	
	//RESET COUNTER
	TIM_SetCounter(TIM4, 0);
	TIM_SetCounter(TIM3, 0);
	
	// 使能计数器
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
