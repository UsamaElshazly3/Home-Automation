/* Login System !  */
/* LIB */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#define	 F_CPU 8000000UL
#include <util/delay.h>
/*MCAL*/

#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/TIMER/TIMERS_private.h"
/*HAL*/
#include "../HAL/LCD/LCD_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../HAL/KEYPAD/KPD_interface.h"
#include "Functions.h"
u8 Local_Au8PassArr[]={'1','2','3','4'};
u8 Local_Au8EnteredNums[4];
u8 Local_u8Flag= 1,i,Local_u8_GasFlag,Local_u8_FlameFlag;
u16 Local_u16DigitalValue,Local_u16AnalogValue,Local_u16TempValue;

void LightSensorCheck(void)
{
	u16 Local_u16DigitalValue, Local_u16AnalogValue;
	ADC_u8GetDigitalValue(ADC_u8_CHANNEL_1,&Local_u16DigitalValue);
		Local_u16AnalogValue = (u16) ((Local_u16DigitalValue * 5000UL) / 1024);
		if(Local_u16AnalogValue > 1500 )
		{
			DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN5, DIO_u8_HIGH);

		}
		else
		{
			DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN5, DIO_u8_LOW);

		}
}
void ShowTemp(void)
{
	LCD_voidClearScreen();
	LCD_u8GoToXY(LCD_u8_LINE1, 0);
	LCD_voidSendString("TEMP:");
	ADC_u8GetDigitalValue(ADC_u8_CHANNEL_0, &Local_u16DigitalValue);
	Local_u16AnalogValue = (u16)((Local_u16DigitalValue * 5000UL) / 1024);
	Local_u16TempValue = Local_u16AnalogValue / 10;
	LCD_voidSendNum(Local_u16TempValue);
	_delay_ms(1000);
	DashBoard();
}
void DashBoard (void)
{
	u8 Local_PressFlag=0,Local_u8Key;
	LCD_voidClearScreen();
	Reset();
	LCD_u8GoToXY(LCD_u8_LINE1, 0);
	LCD_voidSendString("1.Show Temp");
	LCD_u8GoToXY(LCD_u8_LINE2, 0);
	LCD_voidSendString("2.Light");
	LCD_u8GoToXY(LCD_u8_LINE2, 9);
	LCD_voidSendString("3.Exit");
	while(Local_PressFlag==0)
	{
		 DIO_u8GetPinValue(DIO_u8_PORTD,DIO_u8_PIN2,&Local_u8_GasFlag);
		DIO_u8GetPinValue(DIO_u8_PORTD,DIO_u8_PIN3,&Local_u8_FlameFlag);
		checkTemp();
		if(!Local_u8_FlameFlag)
		{
			LCD_voidClearScreen();
			LCD_voidSendString("Fire!!!!");
			Alert(1);
			OpenDoor();
			while(1);
		}
		if(!Local_u8_GasFlag)
		{
			LCD_voidClearScreen();
			LCD_voidSendString("Gas Leaking!!!!");
			Alert(1);
			OpenDoor();
			while(1);
		}
		LightSensorCheck();
		KPD_u8GetKeyState(&Local_u8Key);//Key Pressed Function
		if(Local_u8Key != KPD_u8_KEY_NOT_PRESSED)//If A Key Pressed
		{
			//LightSensorCheck();
			//LCD_voidClearScreen();
			switch(Local_u8Key)
			{
			case '1': 
				ShowTemp();
				break;
			case '2':
				LightControl();
				break;
			case '3':
				break;
			default :
			LCD_voidClearScreen();
			LCD_u8GoToXY(LCD_u8_LINE1, 0);
			LCD_voidSendString("Wrong Choice !");
			_delay_ms(500);
			DashBoard();
			}
			Local_PressFlag =1;
		}
	}
}
void OpenDoor(void)
{
	TIMERS_u16_OCR1A_REG=2500;
}
void CloseDoor(void)
{
	TIMERS_u16_OCR1A_REG=1550;
}
void Alert(u8 check)
{
	if(check==1)
		 DIO_u8SetPinValue(DIO_u8_PORTD, DIO_u8_PIN6, DIO_u8_HIGH);
	else
		 DIO_u8SetPinValue(DIO_u8_PORTD, DIO_u8_PIN6, DIO_u8_LOW);
}
void GreenLed(u8 check)
{
	if(check==1)
		 DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN4, DIO_u8_HIGH);
	else
		 DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN4, DIO_u8_LOW);
}
void RedLed(u8 check)
{
	if(check==1)
		 DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN5, DIO_u8_HIGH);
	else
		 DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN5, DIO_u8_LOW);
}

void AcssesAllowed(void)
{
	 LCD_voidClearScreen();
	 LCD_u8GoToXY(LCD_u8_LINE1, 0);
	 LCD_voidSendString("Access Allowed");
	 OpenDoor();
	 GreenLed(1);
	 _delay_ms(1000);
	 CloseDoor();
	 GreenLed(0);
}
void AccessDenied(void)
{
	 LCD_voidClearScreen();
	 LCD_u8GoToXY(LCD_u8_LINE1, 0);
	 LCD_voidSendString("Access Denied");
	 Alert(1);
	 _delay_ms(1000);
	 Alert(0);

}
void NewTry(void)
{
	// Local_u8Flag = 1;
	 LCD_u8GoToXY(LCD_u8_LINE1, 0);
	 LCD_voidSendString("Enter Password:");
	 LCD_u8GoToXY(LCD_u8_LINE2, 0);
}
void WrongPass(void)
{
	 LCD_voidClearScreen();
	 LCD_u8GoToXY(LCD_u8_LINE1, 0);
	 LCD_voidSendString("Wrong Pass !");
	 RedLed(1);
	 _delay_ms(500);
	 LCD_voidClearScreen();
	 RedLed(0);
	 LCD_u8GoToXY(LCD_u8_LINE1, 0);
	 LCD_voidSendString("Tries Left:");
}
void Reset(void)
{
	 RedLed(0);
	 GreenLed(0);
	 Alert(0);
	 LCD_voidClearScreen();
}
void Login(void)
{
	u8 Local_u8Key;
	_delay_ms(200);
	Reset();
	for(int j=2;j>=0;j--)//The Three Tries Four Loop
	{
		Local_u8Flag = 1;
		NewTry();

	 i=0;
	 while(i<4)//While Loop To Enter The 4 Number Password
	 {
		 DIO_u8GetPinValue(DIO_u8_PORTD,DIO_u8_PIN2,&Local_u8_GasFlag);
		DIO_u8GetPinValue(DIO_u8_PORTD,DIO_u8_PIN3,&Local_u8_FlameFlag);
		if(!Local_u8_FlameFlag)
		{
			LCD_voidClearScreen();
			LCD_voidSendString("Fire!!!!");
			Alert(1);
			OpenDoor();
			while(1);
		}
		if(!Local_u8_GasFlag)
		{
			LCD_voidClearScreen();
			LCD_voidSendString("Gas Leaking!!!!");
			Alert(1);
			OpenDoor();
			_delay_ms(1000);
			while(1);
		}
		 LightSensorCheck();
		 KPD_u8GetKeyState(&Local_u8Key);//Key Pressed Function
		 if(Local_u8Key != KPD_u8_KEY_NOT_PRESSED)//If A Key Pressed
		 {
			 LCD_voidSendChar('*');	// Write On The LCD *
			 Local_Au8EnteredNums[i] = Local_u8Key;// Store Every Char In Array To Compare It Later
			 i++;// The Counter Will Increase Only When He Press A Key.
		}
	 }
	 for(i=0;i<4;i++)// For Loop To Check Every Number In Password Array
	 {
		 if(Local_Au8PassArr[i] != Local_Au8EnteredNums[i])//If Any Number Wasn't Match the password
		 {
				 Local_u8Flag = 0;//Change The Flag
		 }
	 }
		 if(Local_u8Flag == 1) // If The Password Was Correct
		 {
			 AcssesAllowed();
			 DashBoard();
			 break;
		 }
		 else
		 {
			 WrongPass();
			 if(j==0)
			 {
				 LCD_voidSendChar('0');
			 }
			 LCD_voidSendNum(j);//To Display The Left Tries on the screen
			 _delay_ms(500);
		 }
		 if(j==0)
		 {
			 AccessDenied();
		 }
	}
}

void LightControl(void){
	LCD_voidClearScreen();
	u8 Local_PressFlag=0,Local_u8Key;
	LCD_u8GoToXY(LCD_u8_LINE1, 0);
	LCD_voidSendString("1.Turn on");
	LCD_u8GoToXY(LCD_u8_LINE2, 0);
	LCD_voidSendString("2.Turn off");
	while(Local_PressFlag==0)
	{
		DIO_u8GetPinValue(DIO_u8_PORTD,DIO_u8_PIN2,&Local_u8_GasFlag);
		DIO_u8GetPinValue(DIO_u8_PORTD,DIO_u8_PIN3,&Local_u8_FlameFlag);
		checkTemp();
		if(!Local_u8_FlameFlag)
		{
			LCD_voidClearScreen();
			LCD_voidSendString("Fire!!!!");
			Alert(1);
			OpenDoor();
			while(1);
		}
		if(!Local_u8_GasFlag)
		{
			LCD_voidClearScreen();
			LCD_voidSendString("Gas Leaking!!!!");
			Alert(1);
			OpenDoor();
			while(1);
		}
		LightSensorCheck();
		KPD_u8GetKeyState(&Local_u8Key);//Key Pressed Function
		if(Local_u8Key != KPD_u8_KEY_NOT_PRESSED)//If A Key Pressed
		{
			//LightSensorCheck();
			//LCD_voidClearScreen();
			switch(Local_u8Key)
			{
				case '1':
				DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN2, DIO_u8_HIGH);
				break;
				case '2':
				DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN2, DIO_u8_LOW);
				break;
				default :
				LCD_voidClearScreen();
				LCD_u8GoToXY(LCD_u8_LINE1, 0);
				LCD_voidSendString("Wrong Choice !");
				_delay_ms(500);
				DashBoard();
			}
			DashBoard();
			Local_PressFlag =1;
		}
	}
}
void openFan(f32 Duty)
{
	PWM_INIT_FAST();
	f32 local_u8DutyValue = 0 ;
	DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN6, DIO_u8_HIGH);
	DIO_u8SetPinValue(DIO_u8_PORTA, DIO_u8_PIN7, DIO_u8_LOW);
	local_u8DutyValue=((Duty/100) * 255);
	PWM_CTC_VALUE(local_u8DutyValue);
	_delay_ms(100);
}
void closeFan(void)
{
	PWM_CTC_VALUE(0);
}
void checkTemp(void){
	ADC_u8GetDigitalValue(ADC_u8_CHANNEL_0, &Local_u16DigitalValue);
	Local_u16AnalogValue = (u16)((Local_u16DigitalValue * 5000UL) / 1024);
	Local_u16TempValue = Local_u16AnalogValue / 10;
	
	if(Local_u16TempValue >= 25 && Local_u16TempValue < 30){
		openFan(50);
	}
	else if (Local_u16TempValue >= 30 && Local_u16TempValue < 35){
		openFan(70);
	}
	else if (Local_u16TempValue >= 35){
		openFan(100);
	}
	else{
		closeFan();
	}
}

