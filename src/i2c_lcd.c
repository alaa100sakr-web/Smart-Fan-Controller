#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "i2c_lcd.h"

#define LCD_ADDR 0x27

void I2C_Start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	while(!(TWCR & (1<<TWINT)));
}

void I2C_Stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void I2C_Write(unsigned char data)
{
	TWDR = data;

	TWCR = (1<<TWINT)|(1<<TWEN);

	while(!(TWCR & (1<<TWINT)));
}

void LCD_Send(unsigned char data, unsigned char rs)
{
	unsigned char high = data & 0xF0;

	unsigned char low = (data << 4) & 0xF0;

	I2C_Start();

	I2C_Write(LCD_ADDR << 1);

	I2C_Write(high | rs | 0x08 | 0x04);

	_delay_ms(1);

	I2C_Write(high | rs | 0x08);

	I2C_Write(low | rs | 0x08 | 0x04);

	_delay_ms(1);

	I2C_Write(low | rs | 0x08);

	I2C_Stop();

	_delay_ms(2);
}

void LCD_Command(unsigned char cmd)
{
	LCD_Send(cmd, 0);
}

void LCD_Char(unsigned char data)
{
	LCD_Send(data, 1);
}

void LCD_Init()
{
	_delay_ms(50);

	LCD_Command(0x33);

	LCD_Command(0x32);

	LCD_Command(0x28);

	LCD_Command(0x0C);

	LCD_Command(0x06);

	LCD_Command(0x01);

	_delay_ms(5);
}

void LCD_Clear()
{
	LCD_Command(0x01);

	_delay_ms(2);
}

void LCD_String(char *str)
{
	while(*str)
	{
		LCD_Char(*str++);
	}
}

void LCD_SetCursor(char row, char col)
{
	char pos;

	if(row == 0)
	pos = 0x80 + col;
	else
	pos = 0xC0 + col;

	LCD_Command(pos);
}/*
 * i2c_lcd.c
 *
 * Created: 5/12/2026 1:32:49 AM
 *  Author: C.M
 */ 
