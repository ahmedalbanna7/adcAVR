/*
 * ATmega32A_FirmWare.c
 *
 * Created: 8/4/2019 9:17:00 PM
 *  Author: Ahmed
 */ 
#include <ATmega32A_GPIO.h>
#include <LCD.h>
#include <ATmega32a_ADC.h>
#include <AvoMeter_Max5v.h>
#include <Lm35.h>
#include <eeprom.h>
#include <Atmega32A_Interrupt.h>

	void int0_Event(void);
	vuint8_t num = 0x01 ;
	vuint8_t numm = 0x0B ;
	vuint32_t buffer = 0;
	vuint32_t buffer2 = 0;
	vuint8_t test = 0 ;
	void ep(void);
	void adcr(void);
	void hand(void);


int main(void)
{	
	vuint8_t pinselect = 0;
	LCD_init();
	ADC_Init(AREF_Refernce,ADC_Polling,ADC_SingleConv,ADC_Prescale_128);

	GPIO_DirectionSet(GPIOC,PIN0,OUTPUT);
	GPIO_DirectionSet(GPIOC,PIN1,OUTPUT);
	
	GPIO_DirectionSet(GPIOD,PIN4,OUTPUT);

	GPIO_DirectionSet(GPIOA,PIN6,OUTPUT);
	GPIO_DirectionSet(GPIOA,PIN7,OUTPUT);

	Ext_InterruptEnable(Interrupt0,BothEdges);
	ISR0_ptr=int0_Event;
	
	GLOBAL_INETERRUPT_ENABLE();

	while(1)
    {	
			 num = 0x01 ;
			 numm = 0x0B ;
			 pinselect = 0;
			if (test == 1 )
			{	
				GPIO_OutputHandle(GPIOC,PIN0,HIGH);
				GPIO_OutputHandle(GPIOC,PIN1,LOW);
			}
			else
			{
			GPIO_OutputHandle(GPIOC,PIN0,LOW);
			GPIO_OutputHandle(GPIOC,PIN1,LOW);
			}
			GPIO_OutputHandle(GPIOA,PIN6,LOW);
			GPIO_OutputHandle(GPIOA,PIN7,HIGH);

			LCD_Select_RowCol(0,0);
			LCD_DisplayString(" 1 --> meager");
			LCD_Select_RowCol(1,0);
			LCD_DisplayString(" 2 --> hist");
			_delay_ms(200);
			while(GPIO_InputHandle(GPIOD,PIN7) == HIGH)
			{
			LCD_Select_RowCol(2,0);
			LCD_DisplayString("count");
			LCD_Select_RowCol(3,0);
			LCD_DisplayString("Select");
			
			if (GPIO_InputHandle(GPIOD,PIN5) != HIGH)
			{
			pinselect++;
			LCD_Select_RowCol(2,14);
			LCD_DisplayInt(pinselect);
			_delay_ms(200);
			}
			
			}

			LCD_Clear();
			

			if (pinselect == 1)
			{
			LCD_Select_RowCol(1,5);
			LCD_DisplayString("mager");
			_delay_ms(500);
			adcr();
			}
			
			if (pinselect == 2)
			{
			//ep();
			LCD_Select_RowCol(1,5);
			LCD_DisplayString("eeprom");
			_delay_ms(500);
			LCD_Clear();
			hand();
			}
		if (pinselect > 2)
		{
		LCD_Select_RowCol(1,2);
		LCD_DisplayString("stop");
		LCD_Clear();
		_delay_ms(500);
		}
		if (pinselect < 0)
		{
				LCD_DisplayString("wooooooooooo");
				LCD_Clear();
				_delay_ms(500);
		}
			
		
    }
}

void adcr(void)
{
		
			float32 Voltage=0;
			vuint8_t counter = 1;
			while (GPIO_InputHandle(GPIOD,PIN7) != LOW && num != 0x0B)
			{
			LCD_Clear();
			Voltage = ReadVolt(ADC0,Resolution_10Bit,3.0);
			buffer = Voltage ;
			buffer2 =  ((uint32_t)(100*Voltage))%100;
			LCD_DisplayString("Volt = ");
			LCD_DisplayInt(Voltage);
			LCD_DisplayString(".");
			LCD_DisplayInt(((uint32_t)(100*Voltage))%100);
			LCD_DisplayString("V");
			if (ReadVolt(ADC0,Resolution_10Bit,3.0) >= 2.6)
			{
				GPIO_OutputHandle(GPIOD,PIN4,HIGH);
				test = 1 ;
			}	
			if (ReadVolt(ADC0,Resolution_10Bit,3.0) < 2.6)
			{
				GPIO_OutputHandle(GPIOC,PIN0,HIGH);
				GPIO_OutputHandle(GPIOC,PIN1,HIGH);
				test = 0;
			}
			_delay_ms(100);
			ep();
			/*  */
			//volt3 = ((uint32_t)(1000*Voltage))%1000 ;
			//sprintf(buffer2, "%d ", volt3);
			//sprintf(buffer, "%d ", volt2);
			LCD_Select_RowCol(2,4);
			LCD_DisplayString("adc num");
			LCD_DisplayInt(counter);
			LCD_Select_RowCol(3,0);
			LCD_DisplayString("press select <--");
			counter ++ ;
			_delay_ms(500);
			LCD_Clear();
			}
			num = 0x01;
			numm = 0x0B;
			LCD_Clear();
}

void ep(void)
{
		//vuint32_t ad = 0 , ad2 = 0 ;
		//LCD_Clear();
		//Save the  in EEPROM (In location from 1 to 10)
		eeprom_write(num,buffer);
		//Save the  in EEPROM (In location from 1 to 10)
		eeprom_write(numm,buffer2);
		//ad = eeprom_read(num);
		//ad2 = eeprom_read(numm);
		//LCD_Select_RowCol(2,0);
		//LCD_DisplayString("eeprom = ");
		//LCD_DisplayInt(ad);
		//LCD_DisplayString(".");
		//LCD_DisplayInt(ad2);
		//counter++ ;
		num++;
		numm++;
		//_delay_ms(500);
		//LCD_Clear();
}

void hand(void)
{
	uint8_t i = 0;
	int fg = 0;
	int fg2 = 0;
	GPIO_OutputHandle(GPIOC,PIN0,LOW);
	GPIO_OutputHandle(GPIOC,PIN1,HIGH);
	
	while (GPIO_InputHandle(GPIOD,PIN7) != LOW && i != 10 )
	{
	fg = eeprom_read(num+i);
	fg2= eeprom_read(numm+i);
	LCD_Select_RowCol(1,5);
	LCD_DisplayString("EEPROM");
	LCD_Select_RowCol(2,6);
	LCD_DisplayInt(fg);
	LCD_DisplayString(".");
	LCD_DisplayInt(fg2);
	i++;
	LCD_Select_RowCol(3,0);
	LCD_DisplayString("press select <--");
	_delay_ms(500);
	LCD_Clear();
	}
	num = 0x01;
	numm = 0x0B;
}


void int0_Event(void)
{
	GPIO_OutputHandle(GPIOC,PIN0,HIGH);
	GPIO_OutputHandle(GPIOC,PIN1,LOW);
}
