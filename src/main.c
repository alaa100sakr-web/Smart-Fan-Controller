#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "i2c_lcd.h"
#include "DHT11.h"

char buffer[16];
char bluetooth;
char mode = 'A';

uint8_t temp;
uint8_t hum;

//================ I2C =================
void I2C_Init()
{
	TWBR = 32;
	TWSR = 0x00;
}

//================ PWM =================
void PWM_Init()
{
	// OC1A = PD5
	DDRD |= (1<<PD5);

	// Fast PWM 8-bit
	TCCR1A = (1<<COM1A1) | (1<<WGM10);

	TCCR1B = (1<<WGM12) | (1<<CS11);

	OCR1A = 0;
}

//================ UART =================
void UART_Init()
{
	unsigned int ubrr = 51;

	UBRRH = (ubrr >> 8);
	UBRRL = ubrr;

	UCSRB = (1<<TXEN) | (1<<RXEN);

	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
}

char UART_Receive()
{
	if(UCSRA & (1<<RXC))
	{
		return UDR;
	}

	return 0;
}

//================ MAIN =================
int main(void)
{
	//========== INIT ==========
	I2C_Init();

	LCD_Init();

	PWM_Init();

	UART_Init();

	//========== MOTOR DRIVER ==========
	// IN1 = PC2
	// IN2 = PC3

	DDRC |= (1<<PC2) | (1<<PC3);

	// ????? ????
	PORTC |= (1<<PC2);

	PORTC &= ~(1<<PC3);

	LCD_Clear();

	while(1)
	{
		//========== Bluetooth ==========
		bluetooth = UART_Receive();

		if(bluetooth != 0)
		{
			mode = bluetooth;
		}

		//========== DHT11 ==========
		if(DHT11_ReadData(&temp, &hum))
		{
			//========== FIRST LINE ==========
			LCD_SetCursor(0,0);

			LCD_String("T=");

			itoa(temp, buffer, 10);

			LCD_String(buffer);

			LCD_String("C ");

			LCD_String("M=");

			LCD_Char(mode);

			LCD_String("   ");

			//========== SECOND LINE ==========

			//========== MANUAL ==========
			if(mode == '0')
			{
				OCR1A = 0;

				LCD_SetCursor(1,0);

				LCD_String("FAN OFF        ");
			}

			else if(mode == '1')
			{
				OCR1A = 120;

				LCD_SetCursor(1,0);

				LCD_String("LOW SPEED      ");
			}

			else if(mode == '2')
			{
				OCR1A = 180;

				LCD_SetCursor(1,0);

				LCD_String("MED SPEED      ");
			}

			else if(mode == '3')
			{
				OCR1A = 255;

				LCD_SetCursor(1,0);

				LCD_String("HIGH SPEED     ");
			}

			//========== AUTO ==========
			else if(mode == 'A')
			{
				if(temp < 25)
				{
					OCR1A = 0;

					LCD_SetCursor(1,0);

					LCD_String("AUTO OFF       ");
				}

				else if(temp < 35)
				{
					OCR1A = 180;

					LCD_SetCursor(1,0);

					LCD_String("AUTO MED       ");
				}

				else
				{
					OCR1A = 255;

					LCD_SetCursor(1,0);

					LCD_String("AUTO HIGH      ");
				}
			}
		}

		else
		{
			LCD_SetCursor(0,0);

			LCD_String("Sensor Error   ");
		}

		_delay_ms(1000);
	}
}
