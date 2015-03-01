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
//NOTE: \r\n line break style for HTTP headers, RFC2616
//NOTE: \r\n\r\n

void sendResponseHeader();
void sendHtmlBody();
void detHrdEnd(uint8_t data); //searches for 

void startSimpleWebServer(uint8_t socReg){

///////////now start the real stuff
	char stringIn[100];
//	wiznetInitAll();
//	waitForEstablished(socReg);
	char lastChar;
	while(1)
	{
		waitForEstablished(socReg);
		getNewToken(socReg, '\r', stringIn); // this eats up the 'r'
		lastChar = processString(stringIn, 1);
		if(lastChar == '\r'){
			//sendString("**we have a return**");
		}else if(lastChar == '\n' ){
			sendString("**we have new line**"); // signal for header over
//			sendOutSting(SOC0_REG,"ernie was here");
			sendResponseHeader();
			sendHtmlBody();
			endSession(SOC0_REG);
		}
	}
	
	return;
}

void sendResponseHeader(){
	sendOutString(SOC0_REG,"HTTP/1.0 200 OK\r\n");
	sendOutString(SOC0_REG,"Content-Type: text/html\r\n\r\n");
	return;
}
void sendHtmlBody(){
	sendOutString(SOC0_REG,"<!DOCTYPE html>\r\n");
	sendOutString(SOC0_REG,"<html>\r\n");
	sendOutString(SOC0_REG,"<head>\r\n");
	sendOutString(SOC0_REG,"<title>Page Title</title>\r\n");
	sendOutString(SOC0_REG,"</head>\r\n");
	sendOutString(SOC0_REG,"<body>\r\n");
	sendOutString(SOC0_REG,"<h1>And the time is:</h1>\r\n");
	sendTimeBody();
	sendOutString(SOC0_REG,"<p>thank you.</p>\r\n");
	sendOutString(SOC0_REG,"</body>\r\n");
	sendOutString(SOC0_REG,"</html>\r\n\r\n");
	return;
}

void sendTimeBody(){
	uint8_t rtcRegisters[7] = { 0 };
	char timeBuffer[] = "00:00:00 ";
	char dayBuffer[] = "Sun ";
	char dateBuffer[] = "00/00/00 ";

	writePtrReadBytes(SLAVE_ADDRESS_W, SLAVE_ADDRESS_R, 0, rtcRegisters, 7);
	sendOutString(SOC0_REG,"<h1>");
	sendOutString(SOC0_REG, timeStr ( timeBuffer, rtcRegisters ));
	sendOutString(SOC0_REG, dayStr ( dayBuffer, rtcRegisters ));
	sendOutString(SOC0_REG, dateStr ( dateBuffer, rtcRegisters ));
	sendOutString(SOC0_REG,"</h1>");
}