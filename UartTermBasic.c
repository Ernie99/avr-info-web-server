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