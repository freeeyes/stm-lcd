#include "system.h"
#include "usart.h"	 		   
#include "uip.h"	    
#include "enc28j60.h"
#include "tcp_demo.h"


//TCPӦ�ýӿں���(UIP_APPCALL)
//���TCP����(����server��client)��HTTP����
void tcp_demo_appcall(void)
{	
  	
	switch(uip_conn->lport)//���ؼ����˿�1200 
	{
		case HTONS(TCP_LISTEN_PORT):
		    tcp_server_demo_appcall(); 
			break;
		default:						  
		    break;
	}		    
	switch(uip_conn->rport)	//Զ������1400�˿�
	{
	    case HTONS(1400):
			tcp_client_demo_appcall();
	       break;
	    default: 
	       break;
	}   
}
//��ӡ��־��
void uip_log(char *m)
{			    
	printf("uIP log:%s\r\n",m);
}

























