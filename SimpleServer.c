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

///////////now start the real stuff
	char yay[100];
//	wiznetInitAll();
	waitForEstablished(socReg);
	while(1)
	{
		getNewToken(socReg, ' ', yay);
		sendString(yay);
	}
	
	return;
}