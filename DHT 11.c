#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "DHT11.h"

#define DHT_DDR  DDRA
#define DHT_PORT PORTA
#define DHT_PIN  PINA
#define DHT_DATA PA0

uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity)
{
	uint8_t data[5] = {0};
	uint8_t i, j;

	// Start signal
	DHT_DDR |= (1<<DHT_DATA);
	DHT_PORT &= ~(1<<DHT_DATA);
	_delay_ms(20);

	DHT_PORT |= (1<<DHT_DATA);
	_delay_us(30);

	DHT_DDR &= ~(1<<DHT_DATA);

	if(DHT_PIN & (1<<DHT_DATA)) return 0;
	while(!(DHT_PIN & (1<<DHT_DATA)));
	while(DHT_PIN & (1<<DHT_DATA));

	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			while(!(DHT_PIN & (1<<DHT_DATA)));
			_delay_us(30);

			if(DHT_PIN & (1<<DHT_DATA))
			data[i] |= (1<<(7-j));

			while(DHT_PIN & (1<<DHT_DATA));
		}
	}

	*humidity = data[0];
	*temperature = data[2];

	return 1;
}/*
 * DHT_11.c
 *
 * Created: 5/12/2026 1:27:46 AM
 *  Author: C.M
 */ 
