#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tftlcd.h"
#include "led.h"
#include "ring.h"
#include "key.h"

void Screen_Display(u8 pos)
{
	char szData[30] = {'\0'};
	sprintf(szData, "freeeyes(%d)", pos);
	//LCD_ShowString(10, col,tftlcd_data.width,tftlcd_data.height,16, (u8* )szData);
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
	u8 i              = 0; 
	u8 key            = 0;
	char szOutput[30] = {'\0'};
	
	LED_Init();
	SysTick_Init(72);
	delay_init();	    
	uart_init(9600);
	TFTLCD_Init(); 
	KEY_Init();
	 
	screen_log_init();
	 
	while(1)
	{	
		delay_ms(1000);		  
		Screen_Display(i);
		
		LED_Clear();
		Set_Led_Number(i);
		
		screen_log_show();
		
		key=KEY_Scan(0);   //É¨Ãè°´¼ü
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
	}
 }

