#include "wdg.h"
#include "stm32f10x_iwdg.h"

void IWDG_Init(u8 pre,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
	IWDG_SetPrescaler(pre);
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter(); 
	IWDG_Enable();
	
}
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();//reload										   
}


