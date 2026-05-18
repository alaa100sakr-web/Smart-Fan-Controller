#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include <avr/io.h>

void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(char *str);
void LCD_SetCursor(char row, char col);
void LCD_Clear(void);

#endif/*
 * i2c_lcd.h
 *
 * Created: 5/12/2026 1:31:41 AM
 *  Author: C.M
 */ 


#ifndef I2C_LCD_H_
#define I2C_LCD_H_





#endif /* I2C_LCD_H_ */