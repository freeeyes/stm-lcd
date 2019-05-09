#include "led.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_0);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_1);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_2);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_3);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_4);	

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_5);	
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_6); 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_7); 
}

void LED_Clear()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);	
}

void LED_Output_0()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
}

void LED_Output_1()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
}

void LED_Output_2()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}

void LED_Output_3()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}
 
void LED_Output_4()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
}
 
void LED_Output_5()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}
 
void LED_Output_6()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}
 
void LED_Output_7()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
}

void LED_Output_8()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
}

void LED_Output_9()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
}
 
 void Set_Led_Number(u8 number)
 {
	 u8 currnum = number % 10;
	 if(currnum == 0)
	 {
		 LED_Output_0();
	 }
	 else if(currnum == 1)
	 {
		 LED_Output_1();
	 }
	 else if(currnum == 2)
	 {
		 LED_Output_2();
	 }
	 else if(currnum == 3)
	 {
		 LED_Output_3();
	 }	 
	 else if(currnum == 4)
	 {
		 LED_Output_4();
	 }	 	 
	 else if(currnum == 5)
	 {
		 LED_Output_5();
	 }	 	 
	 else if(currnum == 6)
	 {
		 LED_Output_6();
	 }
	 else if(currnum == 7)
	 {
		 LED_Output_7();
	 }
	 else if(currnum == 8)
	 {
		 LED_Output_8();
	 }
	 else if(currnum == 9)
	 {
		 LED_Output_9();
	 }	 	 
 }
