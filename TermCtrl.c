#include <avr/io.h>
#include "StartUp.h"

void initUart()
{
	UBRR0L = 25;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 		// enable Rx & Tx
	UCSR0C=  (1<<UCSZ01)|(1<<UCSZ00);  	       // config USART; 8N1
}

char waitForKey()
{
	char receivedByte = 0; // null
	while ((receivedByte != '\r')&&(receivedByte != 's'))
	{
		while ((UCSR0A & _BV(RXC0)) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR
		receivedByte = UDR0; // Fetch the received byte value into the variable "ReceivedByte"	
	}
	return receivedByte;
}
void sendChar(char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
	UDR0 = c; // Send out the byte value in the variable "ByteToSend"
}

void sendString(char str[])
{
	int index = 0;
	while(str[index]!='\0'){
		sendChar(str[index]);
		index++;
		
	}
}

void ansi_cl(void)
{
	// ANSI clear screen: cl=\E[H\E[J
	sendChar(27);
	sendChar('[');
	sendChar('H');
	sendChar(27);
	sendChar('[');
	sendChar('J');
}
void ansi_me(void)
{
	// ANSI turn off all attribute: me=\E[0m
	sendChar(27);
	sendChar('[');
	sendChar('0');
	sendChar('m');
}
