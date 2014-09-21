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

#define SLAVE_ADDRESS_W 0b11010000
#define SLAVE_ADDRESS_R 0b11010001

void printTimeDayDate(void);
void setTimeDayDate(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day,
	uint8_t date, uint8_t month, uint8_t year);

int main(void)
{
	uartInit();
	_delay_ms(500);
//	twiInit();
	wiznetSpiInit();
	_delay_ms(500);
//	ansiPlain();
//	ansiClear();
	sendString("old data is \n");
	W5500_Test();
	W5500_Init();
	sendString("new data is \n");
	W5500_Test();
	testPoll();
	
	//setTimeDayDate(0, 54, 11, 4, 19, 3, 14);
//	W5100_Init();
	
    while(1)
    {
//		_delay_ms(1000);
//		printTimeDayDate();
//		W5100_test();
//strobeCE();
		
		
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