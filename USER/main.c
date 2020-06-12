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
#include "timer.h"				   
#include "math.h" 	
#include "string.h"

#define TIME_DELAY        10
#define LCD_TIME_INTERVAL 100										
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	 	

void uip_polling(void);	 		

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

//uip事件处理函数
//必须将该函数插入用户主循环,循环调用.

void uip_polling(void)
{
	u8 i;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
	if(timer_ok==0)//仅初始化一次
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/2);  //创建1个0.5秒的定时器 
		timer_set(&arp_timer,CLOCK_SECOND*10);	   	//创建1个10秒的定时器 
	}				 
	uip_len=tapdev_read();	//从网络设备读取一个IP包,得到数据长度.uip_len在uip.c中定义
	if(uip_len>0) 			//有数据
	{   
		//处理IP数据包(只有校验通过的IP包才会被接收) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))//是否是IP包? 
		{
			uip_arp_ipin();	//去除以太网头结构，更新ARP表
			uip_input();   	//IP包处理
			//当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
			//需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)		    
			if(uip_len>0)//需要回应数据
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}else if (BUF->type==htons(UIP_ETHTYPE_ARP))//处理arp报文,是否是ARP请求包?
		{
			uip_arp_arpin();
 			//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len(这是2个全局变量)
 			if(uip_len>0)tapdev_send();//需要发送数据,则通过tapdev_send发送	 
		}
	}else if(timer_expired(&periodic_timer))	//0.5秒定时器超时
	{
		timer_reset(&periodic_timer);		//复位0.5秒定时器 
		//轮流处理每个TCP连接, UIP_CONNS缺省是40个  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//处理TCP通信事件  
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
	 		if(uip_len>0)
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
#if UIP_UDP	//UIP_UDP 
		//轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//处理UDP通信事件
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
			if(uip_len > 0)
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
#endif 
		//每隔10秒调用1次ARP定时器函数 用于定期ARP处理,ARP表10秒更新一次，旧的条目会被抛弃
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
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
	uip_init();
	
	screen_log_add(BLUE, "ENC28J60 Init OK");	
	
	//init IP addr
	uip_ipaddr(ipaddr, 172,21,132,80);	//设置本地设置IP地址
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 172,21,132,1); 	//设置网关IP地址(其实就是你路由器的IP地址)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,252,0);	//设置网络掩码
	uip_setnetmask(ipaddr);
		
	screen_log_add(BLUE, "Addr Config OK");	
	
	//set listen
	uip_listen(HTONS(TCP_LISTEN_PORT));
	
	screen_log_show();
	
	while(1)
	{	
		delay_ms(TIME_DELAY);		  
		
		//处理网络信息
		uip_polling();
		
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

