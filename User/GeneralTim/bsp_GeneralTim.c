
#include "bsp_GeneralTim.h"

// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

// �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���ԴΪTIM4
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	// �����ж���ԴΪTIM3
	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // TIM4���벶��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//��ʼ��GPIOB
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;//ѡ��PB6,PB7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	//TIM3���벶��ͨ��GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//��ʼ��GPIOA
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
	TIM_DeInit(TIM4);
  TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM4_TimeBaseStructure);
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM4_TimeBaseStructure.TIM_Period=2500*4-1;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM4_TimeBaseStructure.TIM_Prescaler= 0;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM4_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM4_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM4_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM4, &TIM4_TimeBaseStructure);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM3_TimeBaseStructure);
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM3_TimeBaseStructure.TIM_Period=2500*4-1;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM3_TimeBaseStructure.TIM_Prescaler= 0;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM3_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM3_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM3_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitTypeDef TIM4_ICInitStructure;
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//???????3,???????
  TIM_ICStructInit(&TIM4_ICInitStructure);//���ṹ���е�����ȱʡ����
  TIM4_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲���
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);//��TIM4_ICInitStructure�е�ָ��������ʼ��ΪTIM4
	
	TIM_ICInitTypeDef TIM3_ICInitStructure;
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//???????3,???????
  TIM_ICStructInit(&TIM3_ICInitStructure);//���ṹ���е�����ȱʡ����
  TIM3_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲���
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);//��TIM3_ICInitStructure�е�ָ��������ʼ��ΪTIM3
	
	// ��������жϱ�־λ
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  // ���������ж�  
	TIM_ITConfig (TIM4, TIM_IT_Update, ENABLE);
	TIM_ITConfig (TIM3, TIM_IT_Update, ENABLE);
	
	
	//RESET COUNTER
	TIM_SetCounter(TIM4, 0);
	TIM_SetCounter(TIM3, 0);
	
	// ʹ�ܼ�����
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
