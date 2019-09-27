#ifndef RING_LOG_H
#define RING_LOG_H

#include "stdio.h"
#include "tftlcd.h"
#include "string.h"

#define MAX_ROW_SIZE  5
#define MAX_LINE_SIZE 30
#define LCD_ROW_BEGIN 10
#define LCD_COL_BEGIN 50
#define LCD_COL_HIGTH 20

struct Log_Info
{
	u16 ncolor;
	char m_data[MAX_LINE_SIZE];
};

struct Log_List_t
{
	struct Log_Info info[MAX_LINE_SIZE];
	u8 nMaxRow;
	u8 nBegin;
	u8 nEnd;
	u8 nCycle;
};

static struct Log_List_t log_list;

void screen_log_init(void);

u8 screen_log_add(u16 ncolor, char* plog);

void screen_log_show(void);

#endif

