#include "ring.h"

//add by freeyes
//log output to screen


void screen_log_init()
{
	log_list.nMaxRow = MAX_ROW_SIZE;
	log_list.nBegin  = 0;
	log_list.nEnd    = 0;
	log_list.nCycle  = 0;
}

u8 screen_log_add(u16 ncolor, char* plog)
{	
	char szOutput[30] = {'\0'};
	u16 nLen = strlen(plog);
	if(nLen >= MAX_LINE_SIZE)
	{
		return 1;
	}
	else
	{
		
		u8 nCurrRow = log_list.nBegin;
		
		strcpy(log_list.info[nCurrRow].m_data, plog);
		log_list.info[nCurrRow].ncolor = ncolor;
		
		log_list.nEnd = log_list.nBegin;
		if(log_list.nBegin < MAX_ROW_SIZE - 1)
		{
			log_list.nBegin++;
		}
		else
		{
			log_list.nBegin = 0;
			log_list.nCycle = 1;
		}
		
		sprintf(szOutput, "[Add]<b=%d><e=%d>.", log_list.nBegin, log_list.nEnd);
		FRONT_COLOR = BLACK;
		LCD_ShowString(LCD_ROW_BEGIN, 10, tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);						
		
		return 0;
	}
}

void screen_log_show()
{
	u8 i    = 0;
	u8 nPos = 0;
	u16 col = 0;
	char szOutput[30] = {'\0'};
	
	//LCD_Fill(0, 0, tftlcd_data.width, tftlcd_data.height, BACK_COLOR);
	
	if(log_list.nBegin == log_list.nEnd)
	{
		return;
	}	
	
	if(log_list.nCycle == 1)
	{
		/*		
		sprintf(szOutput, "[Show]<b=%d><e=%d>.", log_list.nBegin, log_list.nEnd);
		FRONT_COLOR = BLACK;
		LCD_ShowString(LCD_ROW_BEGIN, 30, tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);					
		*/
		
		if(log_list.nBegin < log_list.nEnd)
		{				
			for(i = log_list.nBegin; i < MAX_ROW_SIZE; i++)
			{
				col = LCD_COL_BEGIN + LCD_COL_HIGTH * nPos;
				FRONT_COLOR = log_list.info[i].ncolor;
				sprintf(szOutput, "(%d)<%d>%s. b", nPos, i, log_list.info[i].m_data);
				LCD_ShowString(LCD_ROW_BEGIN, col,tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);
				nPos++;
			}
		}
		else
		{
			for(i = log_list.nBegin; i < MAX_ROW_SIZE; i++)
			{
				col = LCD_COL_BEGIN + LCD_COL_HIGTH * nPos;
				FRONT_COLOR = log_list.info[i].ncolor;
				sprintf(szOutput, "(%d)<%d>%s. c", nPos, i, log_list.info[i].m_data);
				LCD_ShowString(LCD_ROW_BEGIN, col,tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);
				nPos++;
			}
			
			for(i = 0; i <= log_list.nEnd; i++)
			{
				col = LCD_COL_BEGIN + LCD_COL_HIGTH * nPos;
				FRONT_COLOR = log_list.info[i].ncolor;
				sprintf(szOutput, "(%d)<%d>%s. c", nPos, i, log_list.info[i].m_data);
				LCD_ShowString(LCD_ROW_BEGIN, col,tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);
				nPos++;
			}
		}
	}
	else
	{
		u8 i = 0;
		u8 nPos = 0;
		for(i = 0; i <= log_list.nEnd; i++)
		{
			col = LCD_COL_BEGIN + LCD_COL_HIGTH * nPos;
			FRONT_COLOR = log_list.info[i].ncolor;
			sprintf(szOutput, "(%d)<%d>%s.a", nPos, i, log_list.info[i].m_data);
			LCD_ShowString(LCD_ROW_BEGIN, col,tftlcd_data.width,tftlcd_data.height,16,(u8 *)szOutput);
			nPos++;
		}
	}
}

