/*
 * SimpleServer.c
 *
 * Created: 10/12/2014 9:02:43 PM
 *  Author: Ernie
 */ 
#define F_CPU 16000000UL
#include "SimpleServer.h"
#include "Wiznet.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "UartTermBasic.h"

void startSimpleWebServer(uint8_t socReg){
	_delay_ms(500);
	wiznetSpiInit();
	_delay_ms(500);
	sendString("bla");
	W5500_Init();
	readWnetAndPrintSettings();
	W5500_Init_Soc(socReg);
	waitForEstablished(socReg);
///////////now start the real stuff
	char yay[100];
	while(1)
	{
		getNewToken(socReg, ' ', yay);
		sendString(yay);
	}
	
	return;
}