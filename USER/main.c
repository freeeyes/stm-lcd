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

//uip�¼�������
//���뽫�ú��������û���ѭ��,ѭ������.

void uip_polling(void)
{
	u8 i;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
	if(timer_ok==0)//����ʼ��һ��
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/2);  //����1��0.5��Ķ�ʱ�� 
		timer_set(&arp_timer,CLOCK_SECOND*10);	   	//����1��10��Ķ�ʱ�� 
	}				 
	uip_len=tapdev_read();	//�������豸��ȡһ��IP��,�õ����ݳ���.uip_len��uip.c�ж���
	if(uip_len>0) 			//������
	{   
		//����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))//�Ƿ���IP��? 
		{
			uip_arp_ipin();	//ȥ����̫��ͷ�ṹ������ARP��
			uip_input();   	//IP������
			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
			//��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)		    
			if(uip_len>0)//��Ҫ��Ӧ����
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}else if (BUF->type==htons(UIP_ETHTYPE_ARP))//����arp����,�Ƿ���ARP�����?
		{
			uip_arp_arpin();
 			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len(����2��ȫ�ֱ���)
 			if(uip_len>0)tapdev_send();//��Ҫ��������,��ͨ��tapdev_send����	 
		}
	}else if(timer_expired(&periodic_timer))	//0.5�붨ʱ����ʱ
	{
		timer_reset(&periodic_timer);		//��λ0.5�붨ʱ�� 
		//��������ÿ��TCP����, UIP_CONNSȱʡ��40��  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//����TCPͨ���¼�  
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
	 		if(uip_len>0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#if UIP_UDP	//UIP_UDP 
		//��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10��
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//����UDPͨ���¼�
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
			if(uip_len > 0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#endif 
		//ÿ��10�����1��ARP��ʱ������ ���ڶ���ARP����,ARP��10�����һ�Σ��ɵ���Ŀ�ᱻ����
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
	uip_ipaddr(ipaddr, 172,21,132,80);	//���ñ�������IP��ַ
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 172,21,132,1); 	//��������IP��ַ(��ʵ������·������IP��ַ)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,252,0);	//������������
	uip_setnetmask(ipaddr);
		
	screen_log_add(BLUE, "Addr Config OK");	
	
	//set listen
	uip_listen(HTONS(TCP_LISTEN_PORT));
	
	screen_log_show();
	
	while(1)
	{	
		delay_ms(TIME_DELAY);		  
		
		//����������Ϣ
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

