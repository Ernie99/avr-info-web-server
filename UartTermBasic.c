/** @file UartTermBasic.c
 *  @brief Implementation, see header for API
 *  @author Ernie H
 */
#include <stdint.h>
#include "UartTermBasic.h"
#include <avr/io.h>

#define UBRR_NUM 103 //9600 at 16MHz
//#define UBRR_NUM 25 //3.4k at 16MHz

//prototypes for private methods
char getInput(void);

void uartInit()
{
	UBRR0L = UBRR_NUM;
	UCSR0B =_BV(RXEN0) | _BV(TXEN0);
}

void sendString(char data[])
{
	uint8_t i = 0;
	while (data[i] != '\0')
	{
		sendChar(data[i]);
		i++;
	}
}

void ansiClear(void)
{
	// ANSI clear screen: cl=\E[H\E[J
	sendChar(27);
	sendChar('[');
	sendChar('H');
	sendChar(27);
	sendChar('[');
	sendChar('J');
}
void ansiPlain(void)
{
	// ANSI turn off all attribute: me=\E[0m
	sendChar(27);
	sendChar('[');
	sendChar('0');
	sendChar('m');
}

void sendChar(char data)
{
	while ((UCSR0A & _BV(UDRE0))==0)
	{}
	UDR0 = data;
}

char getInput(void)
{
	char receivedByte;
	while ((UCSR0A & _BV(RXC0)) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR
	receivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	return receivedByte;
}

void printOctet(uint8_t octet)
{
	uint8_t hundreds = octet / 100;
	octet = octet % 100;
	uint8_t tens = octet / 10;
	octet = octet % 10;
	uint8_t ones = octet;
	sendChar(hundreds + 48);
	sendChar(tens + 48);
	sendChar(ones + 48);
}