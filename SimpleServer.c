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

uint8_t hasString(char * body, char * search);

void startSimpleWebServer(uint8_t socReg){

///////////now start the real stuff
	char yay[100];
	wiznetInitAll();
	sendString(" h1 ");
	waitForEstablished(socReg);
	sendString(" h2 ");
//	waitAndPrintBuffer(socReg);
	while(1)
	{
		getNewTokenCrLf(socReg, yay);
		sendString(yay);
		sendChar('\r'); sendChar('\n');
		
	}
	
	return;
}

void startSimpleWebServerGOOD(uint8_t socReg){

	///////////now start the real stuff
	char buf[130];
	wiznetInitAll();
	sendString(" h1 ");
	waitForEstablished(socReg);
	sendString(" h2 ");
	//	waitAndPrintBuffer(socReg);
	while(1)
	{
		getNewTokenCrLf(socReg, buf);
		sendString(buf);
		sendChar('\r'); sendChar('\n');
		
	}
	
	getNewTokenCrLf(socReg, buf);
	if (hasString)
	{
	}
	// wait for GET / HTTP/1.1
	// wait for CrLf
	// wait for two Cr/Lf
	
	//HTTP/1.0 200 OK
	//Content-Type: text/html

	//Web page
	
	//close socket
	
	return;
}

uint8_t hasString(char * body, char * search) // return word length or 0 if not found
{
	uint8_t indexBody = 0;
	uint8_t indexSearch = 0;
	uint8_t firstHit = 0;
	char searchChar = 0xFF; // not the best way to do this.
	char bodyChar;
	char hit = 0;
	uint8_t indexBodyL;
	
	
	bodyChar=body[indexBody];
	while (bodyChar != '\0') // go thru the body
	{
		indexSearch = 0;
		
		searchChar = search[indexSearch];
		bodyChar = body[indexBody];
		
		indexBodyL = indexBody;
		while(searchChar != '\0') //look for next char hit
		{
			if(searchChar != bodyChar)
			{
				hit = 0;
				break;	
			}
			else
			{
				indexSearch++;
				indexBodyL++;
				//bodyIndex++;
				bodyChar = body[indexBodyL];
				searchChar = search[indexSearch];
				hit = 1;
			}	
		}
		if(hit)
			break;
		indexBody ++;	
	}
	return hit;
}

void testHasString(void)
{
	char test1[] = "wow hi you are cool hi";
	char testA[] = "you";
	
	uint8_t result;
	result = hasString(test1, testA);
	if (result == 1)
	{
		sendString("yes");
	}
	else
	{
		sendString("no");
	}
}