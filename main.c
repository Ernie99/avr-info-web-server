/** @file main.c
 *  @brief Function prototypes and implementation to control web-server
 *  @author Ernie H
 */ 
 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "UartTermBasic.h"
#include "I2c.h"
#include "RtcStrings.h"
#include "Wiznet.h"
#include "SimpleServer.h"
#include "ConfigServer.h"

#define SLAVE_ADDRESS_W 0b11010000
#define SLAVE_ADDRESS_R 0b11010001

void printTimeDayDate(void);
void setTimeDayDate(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day,
	uint8_t date, uint8_t month, uint8_t year);
void setSomeStuff();

int main(void)
{
	uartInit();
	twiInit();
//	setSomeStuff();
//	wiznetInitAll();
	_delay_ms(500);
	wiznetSpiInit();
	_delay_ms(500);
	
	configLoop();
	
	sendChar('\n');
	sendChar('\r');
	printTimeDayDate();
	startSimpleWebServer(SOC0_REG);	
	
    while(1)
    {		
    }
}

void setTimeDayDate(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day,
	uint8_t date, uint8_t month, uint8_t year)
{
	uint8_t data[7] = { 0 };
	data[0] = decToBcd(second);
	data[1] = decToBcd(minute);
	data[2] = decToBcd(hour);
	data[3] = decToBcd(day);
	data[4] = decToBcd(date);
	data[5] = decToBcd(month);
	data[6] = decToBcd(year);
	twiMasterWrite(SLAVE_ADDRESS_W, 0 , data, 7);
}

void printTimeDayDate()
{
	uint8_t rtcRegisters[7] = { 0 };
	char timeBuffer[] = "00:00:00 ";
	char dayBuffer[] = "Sun ";
	char dateBuffer[] = "00/00/00 ";
	
	writePtrReadBytes(SLAVE_ADDRESS_W, SLAVE_ADDRESS_R, 0, rtcRegisters, 7);
	
	sendString(timeStr ( timeBuffer, rtcRegisters ));
	sendString(dayStr ( dayBuffer, rtcRegisters ));
	sendString(dateStr ( dateBuffer, rtcRegisters ));
	sendChar('\n');
	sendChar('\r');
}

void setSomeStuff(){
	uint8_t day = 0x1; // sunday
	uint8_t date = 0b00001000; // 8th
	uint8_t month = 0b00000010; // feb
	
	twiMasterWrite(SLAVE_ADDRESS_W, 3 , &day, 1);
	twiMasterWrite(SLAVE_ADDRESS_W, 4 , &date, 1);
	twiMasterWrite(SLAVE_ADDRESS_W, 5 , &month, 1);
	
//	writePtrReadBytes(SLAVE_ADDRESS_W, SLAVE_ADDRESS_R, )
}