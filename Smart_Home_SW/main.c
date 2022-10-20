/* Login System !  */
/* LIB */
#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#define	 F_CPU 8000000UL
#include <util/delay.h>
/*MCAL*/
#include "MCAL/DIO/DIO_interface.h"
/*HAL*/
#include "MCAL/TIMER/TIMERS_private.h"
#include "HAL/LCD/LCD_interface.h"
#include "MCAL/GI/GI_interface.h"
#include "MCAL/TIMER/TIMERS_interface.h"
#include "HAL/KEYPAD/KPD_interface.h"
#include "MCAL/ADC/ADC_interface.h"
#include "APP/Functions.h"


int main(void)
{
	u8 Local_u8Key;
	DIO_voidInit();
	GI_voidEnable();
	TIMERS_voidTimer0Init();
	ADC_voidInit();
	LCD_voidInit();
	LCD_voidClearScreen();


	while(1)
	{
		Login();
		LCD_voidClearScreen();
		Reset();
		LCD_u8GoToXY(LCD_u8_LINE1, 0);
		LCD_voidSendString("Good Bye !");


	}
	 return 0;
}

