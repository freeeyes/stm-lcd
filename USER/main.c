#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tftlcd.h"
#include "ring.h"

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
	u8 i = 0; 
	
	SysTick_Init(72);
	delay_init();	    
	uart_init(9600);
	TFTLCD_Init(); 
	 
	screen_log_init();
	 
	while(1)
	{	
		delay_ms(1000);		  
		Screen_Display(i);
		
		screen_log_show();
		i++;
	}
 }

