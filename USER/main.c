#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include "tftlcd.h"
#include "led.h"
#include "ring.h"
#include "key.h"
#include "wdg.h"

#include "rtc.h" 
#include "adc.h"
#include "adc_temp.h"
#include "enc28j60.h"
#include "tapdev.h"
#include "uip.h"
#include "uip_arp.h"

#define TIME_DELAY        10
#define LCD_TIME_INTERVAL 100

void Screen_Display(u8 pos)
{
	char szData[30] = {'\0'};
	sprintf(szData, "freeeyes(%d)", pos);
	if(pos % 2 == 0)
	{
		screen_log_add(RED, szData);
	}
	else
	{
		screen_log_add(BLUE, szData);
	}
}

 int main(void)
 { 
	u8  i              = 0; 
	u8  key            = 0;
	u8  nLcdShow       = 0;
	int nSecend        = 0;
	char szOutput[30]  = {'\0'};
	uip_ipaddr_t ipaddr;

	//check dog
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
		RCC_ClearFlag();
  }	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init(); 
	KEY_Init();
	IWDG_Init(IWDG_Prescaler_256, 157);
	 
	screen_log_init();
	
	RTC_Init();
	ADCx_Init();	
	ADC_Temp_Init();
	while(tapdev_init())	
	{								   
		//Init ENC28J60 error
		screen_log_add(RED, "ENC28J60 Init Error!");	 
		printf("ENC28J60 Init Error!\r\n");
		return 0;
	};
	
	screen_log_add(BLUE, "ENC28J60 Init OK");	
	
	//init IP addr
	uip_ipaddr(ipaddr, 192,168,1,41);	
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,1,1); 
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);
		
	screen_log_add(BLUE, "Addr Config OK");	
	
	screen_log_show();
	
	while(1)
	{	
		delay_ms(TIME_DELAY);		  
		
		if(nLcdShow == LCD_TIME_INTERVAL)
		{
			//Screen_Display(nSecend);
			
			LED_Clear();
			Set_Led_Number(nSecend);
			
			//screen_log_show();
			nLcdShow = 0;
			nSecend++;
		}
		else
		{
			nLcdShow++;
		}
		
		key=KEY_Scan(0);   //scan key
		switch(key)
		{
			case KEY_UP: 
			{
				sprintf(szOutput, "[KEY]KEY_UP.");
				FRONT_COLOR = BLACK;
				LCD_ShowString(LCD_ROW_BEGIN, 30, tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);	
				break;				
			}
			case KEY_DOWN: 
			{
				sprintf(szOutput, "[KEY]KEY_DOWN.");
				FRONT_COLOR = BLACK;
				LCD_ShowString(LCD_ROW_BEGIN, 30, tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);		
				break;				
			}
			case KEY_LEFT:
			{
				sprintf(szOutput, "[KEY]KEY_LEFT.");
				FRONT_COLOR = BLACK;
				LCD_ShowString(LCD_ROW_BEGIN, 30, tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);			
				break;								
			}
			case KEY_RIGHT:
			{
				sprintf(szOutput, "[KEY]KEY_RIGHT.");
				FRONT_COLOR = BLACK;
				LCD_ShowString(LCD_ROW_BEGIN, 30, tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);	
				break;				
			}
		}		
		
		i++;
		
		IWDG_Feed();
	}
 }

