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

/**************************************
*         PRIVATE PROTOTYPES          *
***************************************/
void detHrdEnd(uint8_t data); //searches for 

void startSimpleWebServer(uint8_t socReg){

///////////now start the real stuff
	char stringIn[100];
//	wiznetInitAll();
	waitForEstablished(socReg);
	char lastChar;
	while(1)
	{
		getNewToken(socReg, '\r', stringIn); // this eats up the 'r'
		lastChar = processString(stringIn, 1);
		if(lastChar == '\r'){
			sendString("**we have a return**");
		}else if(lastChar == '\n' ){
			sendString("**we have new line**");
		}
	}
	
	return;
}